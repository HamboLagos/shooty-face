#include "AI_enemy.hpp"

#include "A_star.hpp"
#include "game.hpp"
#include "components/physics.hpp"
#include "collision.hpp"
#include "utils.hpp"

void
AIEnemy::refresh(sf::Time frame_length)
{
    auto* physics = get_enemy().get_component<Physics>();
    auto& game = Game::instance();

    if (!game.get_player()) {
        physics->set_velocity(sf::Vector2f{0.f, 0.f});
        fraction_to_player_ = 0.f;
        return;
    }
    const auto* player_physics = game.get_player()->get_component<Physics>();

    const auto map = game.get_map(&get_entity());
    const auto start = game.get_tile_for(physics->get_box().get_min_corner());
    const auto end = game.get_tile_for(player_physics->get_box().get_min_corner());
    const auto dimensions = game.to_tile_dimensions(physics->get_dimensions());

    auto result = AStar::run(start, end, dimensions, map);
    path_ = result.path;
    if (result.has_path && result.path.size() > 1) {
        auto destination = game.get_position_for(result.path[1]);
        auto movement = util::devector(destination - physics->get_box().get_min_corner());
        physics->set_velocity(movement.direction * physics->get_move_speed());
        fraction_to_player_ =
            movement.length / util::length(physics->get_velocity() * frame_length.asSeconds());
    } else {
        auto destination = game.get_player()->get_component<Physics>()->get_position();
        auto movement = util::devector(destination - physics->get_position());
        physics->set_velocity(movement.direction * physics->get_move_speed());
        fraction_to_player_ =
            movement.length / util::length(physics->get_velocity() * frame_length.asSeconds());
    }

    if (util::length(physics->get_velocity()) == 0.f) {
        fraction_to_player_ = 0.f;
    }

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
