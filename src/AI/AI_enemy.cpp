#include "AI_enemy.hpp"

#include "game.hpp"
#include "components/physics.hpp"
#include "collision.hpp"
#include "utils.hpp"

void
AIEnemy::refresh(sf::Time frame_length)
{
    auto* physics = get_enemy().get_component<Physics>();

    // attempt to track the player
    auto* player = Game::instance().get_player();
    if (player == nullptr) {
        fraction_to_player_ = 0.f;
        physics->set_velocity(sf::Vector2f(0.f, 0.f));
        return;
    }

    auto* player_physics = player->get_component<Physics>();
    auto destination = util::devector(player_physics->get_position() - physics->get_position());
    physics->set_velocity(destination.direction * physics->get_move_speed());

    if (util::length(physics->get_velocity()) == 0.f) {
        fraction_to_player_ = 0.f;
        physics->set_velocity(sf::Vector2f(0.f, 0.f));
        return;
    }

    fraction_to_player_ =
        destination.length / util::length(physics->get_velocity() * frame_length.asSeconds());

    // normalize fraction to [0, 1.f]
    fraction_to_player_ = std::min(fraction_to_player_, 1.f);
}

sf::Time
AIEnemy::update(sf::Time elapsed)
{
    auto& enemy = get_enemy();
    if (enemy.is_dead() || fraction_to_player_ == 0.f) {
        return elapsed;
    }

    auto* physics = enemy.get_component<Physics>();
    auto box = physics->get_box(elapsed.asSeconds());
    AABB collision_box({0.f, 0.f}, {0.f, 0.f});

    float min_percent_safe = fraction_to_player_;
    for(auto& entity : Game::instance().entities()) {
        if (!Collision::sanity_check(enemy, *entity)) {
            continue;
        }

        auto entity_box = entity->get_component<Physics>()->get_box();

        if (Collision::broad_test(box, entity_box)) {
            float percent_safe = Collision::narrow_test(box, entity_box);

            if (percent_safe == 0.f) {
                auto unpenetrate = -Collision::get_penetration(box, entity_box);
                physics->move(unpenetrate);
            }

            // save the shortest safe move
            if (percent_safe < min_percent_safe) {
                min_percent_safe = percent_safe;
                collision_box = entity_box;
            }
        }
    }

    sf::Time used = elapsed * min_percent_safe;
    physics->update(used.asSeconds());

    // if we moved and can (maybe) move more
    if (sf::Time::Zero < used && used < elapsed) {

        if (fraction_to_player_ > 0.f) {
            sf::Vector2f new_velocity = physics->get_velocity();

            // expand the the box sligthly and see where it hits the nearest entity
            auto box = AABB::state_space_for(physics->get_box(used.asSeconds() / 10.f));
            auto unpenetrate = Collision::get_penetration(box, collision_box);

            if (unpenetrate.x != 0.f) { new_velocity.x = 0.f; }
            if (unpenetrate.y != 0.f) { new_velocity.y = 0.f; }

            physics->set_velocity(new_velocity);
        }
    }

    return used;
}
