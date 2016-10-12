#include "AI_enemy.hpp"

#include "A_star.hpp"
#include "game.hpp"
#include "components/physics.hpp"
#include "collision.hpp"
#include "utils.hpp"

const float AIEnemy::ASTAR_REFRESH_RATE = 5.f; ///< Hz
int AIEnemy::user_count_ = 0;
int AIEnemy::next_user_ = 0;

void
AIEnemy::prepare()
{
    if (++next_user_ == user_count_) {
        next_user_ = 0;
    }
}

void
AIEnemy::refresh(sf::Time frame_length)
{
    auto* physics = get_enemy().get_component<Physics>();
    auto& game = Game::instance();

    if (!game.get_player()) {
        physics->set_velocity(sf::Vector2f{0.f, 0.f});
        fractional_move_ = 0.f;
        return;
    }
    const auto* player_physics = game.get_player()->get_component<Physics>();
    const float distance_can_move = util::length(physics->get_velocity()) * frame_length.asSeconds();

    if (user_ndx_ == next_user_ && refresh_rate_->check()) {
        refresh_rate_->renew();

        const auto start = game.get_tile_for(physics->get_box().get_min_corner());
        const auto end = game.get_tile_for(player_physics->get_box().get_min_corner());

        auto distance = end - start;
        if (std::abs(distance.x) < 20 && std::abs(distance.y) < 20) {
            const auto dimensions = game.to_tile_dimensions(physics->get_dimensions());
            const auto map = game.get_map(&get_entity());

            auto result = AStar::run(start, end, dimensions, map);
            has_path_ = result.has_path;
            path_ = std::move(result.path);
            path_ndx_ = 1;
        } else {
            has_path_ = false;
            path_ = AStar::Path();
        }
    }

    if (has_path_ && path_.size() > 0) {
        // find the tile we want to move to next, using a whole move where possible
        const auto box = physics->get_box();
        const auto min_corner = box.get_min_corner();

        float distance_will_move = 0.f;
        --path_ndx_;
        while (distance_will_move < distance_can_move) {
            if (++path_ndx_ >= path_.size()) {
                --path_ndx_;
                break;
            }

            const auto tile_box =
                AABB(game.get_position_for(path_[path_ndx_]) + game.get_tile_dimensions()/2.f,
                     game.get_tile_dimensions());
            if (Collision::broad_test(box, tile_box)) {
                continue;
            }

            auto tile_position = game.get_position_for(path_[path_ndx_]);
            distance_will_move = util::length(tile_position - min_corner);
        }

        // set up the move for the next update
        auto destination = game.get_position_for(path_[path_ndx_]);
        auto movement = util::devector(destination - min_corner);

        physics->set_velocity(movement.direction * physics->get_move_speed());
        fractional_move_ = movement.length / distance_can_move;
    } else {
        // try tracking the player
        auto destination = player_physics->get_position();
        auto movement = util::devector(destination - physics->get_position());
        physics->set_velocity(movement.direction * physics->get_move_speed());
        fractional_move_ = movement.length / distance_can_move;
    }

    // normalize fraction to [0, 1.f]
    fractional_move_ = std::min(std::max(0.f, fractional_move_), 1.f);
}

sf::Time
AIEnemy::update(sf::Time elapsed)
{
    auto& enemy = get_enemy();
    if (enemy.is_dead() || fractional_move_ == 0.f) {
        return elapsed;
    }

    auto* physics = enemy.get_component<Physics>();
    auto box = physics->get_box(elapsed.asSeconds());
    AABB collision_box({0.f, 0.f}, {0.f, 0.f});

    float min_percent_safe = fractional_move_;
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
        auto new_velocity = physics->get_velocity();

        // expand the the box sligthly and see where it hits the nearest entity
        auto box = AABB::state_space_for(physics->get_box(used.asSeconds() / 10.f));
        auto unpenetrate = Collision::get_penetration(box, collision_box);

        if (unpenetrate.x != 0.f) { new_velocity.x = 0.f; }
        if (unpenetrate.y != 0.f) { new_velocity.y = 0.f; }

        physics->set_velocity(new_velocity);
    }

    return used;
}
