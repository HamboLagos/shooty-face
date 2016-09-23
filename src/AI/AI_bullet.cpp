#include "AI_bullet.hpp"

#include "collision.hpp"
#include "components/physics.hpp"
#include "game.hpp"

sf::Time
AIBullet::update(sf::Time elapsed)
{
    auto& bullet = get_bullet();
    if (bullet.is_dead()) {
        return elapsed;
    }

    auto* bullet_physics = bullet.get_component<Physics>();
    auto bullet_box = bullet_physics->get_box(elapsed.asSeconds());

    float min_percent_safe = 1.f;
    Entity* damaged_entity = nullptr;
    for(const auto& entity : Game::instance().entities()) {
        if (entity.get() == Game::instance().get_player()) {
            continue;
        }

        if (!Collision::sanity_check(bullet, *entity)) {
            continue;
        }

        auto entity_box = entity->get_component<Physics>()->get_box();

        if (Collision::broad_test(bullet_box, entity_box)) {
            float percent_safe = Collision::narrow_test(bullet_box, entity_box);

            if (percent_safe < min_percent_safe) {
                min_percent_safe = percent_safe;
                damaged_entity = entity.get();
            }
        }
    }

    auto used = elapsed * min_percent_safe;
    bullet_physics->update(used.asSeconds());

    if (min_percent_safe < 1.f) {
        bullet.apply_damage(*damaged_entity);
        bullet.kill();
    }

    return used;
}
