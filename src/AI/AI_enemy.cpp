#include "AI_enemy.hpp"

#include "game.hpp"
#include "components/physics.hpp"
#include "collision.hpp"
#include "utils.hpp"

void
AIEnemy::update(float dt)
{
    auto& enemy = get_enemy();
    if (enemy.is_dead()) {
        return;
    }

    // attempt to track the player
    auto* player = Game::instance().get_player();
    if (player == nullptr) {
        return;
    }

    auto* player_physics = player->get_component<Physics>();
    auto* enemy_physics = enemy.get_component<Physics>();
    auto dir = util::direction(player_physics->get_position() - enemy_physics->get_position());
    enemy_physics->set_velocity(dir * enemy_physics->get_move_speed());

    float min_percent_safe = 1.f;
    auto enemy_box = enemy_physics->get_box(dt);
    for(auto& entity : Game::instance().entities()) {
        if (!Collision::sanity_check(enemy, *entity)) {
            continue;
        }

        auto entity_box = entity->get_component<Physics>()->get_box();

        if (Collision::broad_test(enemy_box, entity_box)) {
            float percent_safe = Collision::narrow_test(enemy_box, entity_box);

            if (percent_safe < 1.f) {
                if (percent_safe == 0.f) {
                    auto unpenetrate = -Collision::get_penetration(enemy_box, entity_box);
                    enemy_physics->move(unpenetrate);
                }

                // save the shortest safe move
                if (percent_safe < min_percent_safe) {
                    min_percent_safe = percent_safe;
                }
            }
        }
    }

    enemy_physics->update(dt * min_percent_safe);
}
