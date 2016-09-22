#include "AI_bullet.hpp"

#include "collision.hpp"
#include "components/physics.hpp"
#include "game.hpp"

void
AIBullet::update(float dt)
{
    auto& bullet = get_bullet();
    if (bullet.is_dead()) {
        return;
    }

    auto* bullet_physics = bullet.get_component<Physics>();
    auto bullet_box = bullet_physics->get_box(dt);

    for(const auto& entity : Game::instance().entities()) {
        /// TODO Add tests for this check
        if (entity.get() == Game::instance().get_player()) {
            continue;
        }

        if (!Collision::sanity_check(bullet, *entity)) {
            continue;
        }

        auto entity_box = entity->get_component<Physics>()->get_box();

        if (Collision::broad_test(bullet_box, entity_box)) {
            float percent_safe = Collision::narrow_test(bullet_box, entity_box);

            /// TODO test for hitting multiple entities
            /// TODO only apply damage to the first entity hit
            if (percent_safe < 1.f) {
                bullet_physics->update(dt * percent_safe);
                bullet.apply_damage(*entity);
                bullet.kill();
                break;
            }
        }
    }

    if (bullet.is_alive()) {
        bullet_physics->update(dt);
    }
}
