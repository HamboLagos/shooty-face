#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "config.h"

#include "game.hpp"
#include "player.hpp"
#include "gun.hpp"
#include "bullet.hpp"
#include "barrier.hpp"
#include "enemy.hpp"

#include "components/physics.hpp"
#include "components/health.hpp"

using namespace std;
constexpr auto WINDOW_WIDTH  = Game::WINDOW_WIDTH;
constexpr auto WINDOW_HEIGHT = Game::WINDOW_WIDTH;

void add_renderings(Renderer::Renderings& to, const Renderer::Renderings& from)
{
    to.insert(to.end(), from.begin(), from.end());
}

int main(int argc, char *argv[])
{
    sf::Font font;
    font.loadFromFile("../fonts/Aileron-Regular.otf");

    sf::Text frame_rate;
    frame_rate.setFont(font);
    frame_rate.setCharacterSize(50);
    frame_rate.setColor(sf::Color::Blue);
    frame_rate.setPosition(sf::Vector2f(0.f, 0.f));

    sf::Text timer;
    timer.setFont(font);
    timer.setCharacterSize(50);
    timer.setColor(sf::Color::Blue);
    timer.setPosition(sf::Vector2f(0.f, 40.f));

    std::cout << "Version " << shooty_face_VERSION_MAJOR
              << "."        << shooty_face_VERSION_MINOR
              << "."        << shooty_face_VERSION_REVIS
              << std::endl;

    auto& game = Game::instance();

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shooty Face");
    /* app.setFramerateLimit(250); */

    auto& player = *game.add_player();
    player.get_component<Physics>()->set_position(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)/2.f);

    game.entity_collection().push_back(std::make_unique<Gun>(player));
    Gun& gun = static_cast<Gun&>(*game.entity_collection().back().get());
    gun.set_ammunition(std::make_unique<BulletAmmunition>());

    std::vector<Barrier*> barriers(4); ///< top, left, bottom, right
    for(auto& barrier : barriers) {
        barrier = new Barrier();
        game.entity_collection().emplace_back(barrier);
    }

    float border_thickness = 20.f;
    barriers[0]->get_component<Physics>()->set_position({WINDOW_WIDTH/2.f, border_thickness/2.f});
    barriers[0]->get_component<Physics>()->set_dimensions({WINDOW_WIDTH, border_thickness});
    barriers[1]->get_component<Physics>()->set_position({border_thickness/2.f, WINDOW_HEIGHT/2.f});
    barriers[1]->get_component<Physics>()->set_dimensions({border_thickness, WINDOW_HEIGHT});
    barriers[2]->get_component<Physics>()->set_position({WINDOW_WIDTH/2.f, WINDOW_HEIGHT - border_thickness/2.f});
    barriers[2]->get_component<Physics>()->set_dimensions({WINDOW_WIDTH, border_thickness});
    barriers[3]->get_component<Physics>()->set_position({WINDOW_WIDTH - border_thickness/2.f, WINDOW_HEIGHT/2.f});
    barriers[3]->get_component<Physics>()->set_dimensions({border_thickness, WINDOW_HEIGHT});

    barriers.clear();

    std::vector<Enemy*> enemies(24);
    int ndx = 0;
    for(auto& enemy : enemies) {
        enemy = new Enemy();
        enemy->get_component<Health>()->set_health(15.f);

        auto* physics = enemy->get_component<Physics>();
        if (ndx++ <= 3) {
            physics->set_move_speed(physics->get_move_speed() * 1.5f);
            physics->set_dimensions(physics->get_dimensions() * 1.5f);
        }

        game.entity_collection().emplace_back(enemy);
    }


    sf::Vector2f zero_offset = {border_thickness + enemies[0]->get_component<Physics>()->get_extents().x,
                                border_thickness + enemies[0]->get_component<Physics>()->get_extents().y};
    enemies[0]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    auto& enemy_example = *enemies[0];
    enemies[1]->get_component<Physics>()->set_position({zero_offset.x, WINDOW_HEIGHT - zero_offset.y});
    enemies[2]->get_component<Physics>()->set_position({WINDOW_WIDTH - zero_offset.x, WINDOW_HEIGHT - zero_offset.y});
    enemies[3]->get_component<Physics>()->set_position({WINDOW_WIDTH - zero_offset.x, zero_offset.y});

    zero_offset = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT/4.f};
    enemies[4]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/16.f, zero_offset.y});
    enemies[5]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[6]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[7]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[8]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/16.f, zero_offset.y});

    zero_offset = {WINDOW_WIDTH/4.f, WINDOW_HEIGHT/2.f};
    enemies[9]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/16.f});
    enemies[10]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/8.f});
    enemies[11]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[12]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/8.f});
    enemies[13]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/16.f});

    zero_offset = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT*3.f/4.f};
    enemies[14]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/16.f, zero_offset.y});
    enemies[15]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[16]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[17]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[18]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/16.f, zero_offset.y});

    zero_offset = {WINDOW_WIDTH*3.f/4.f, WINDOW_HEIGHT/2.f};
    enemies[19]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/16.f});
    enemies[20]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/8.f});
    enemies[21]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[22]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/8.f});
    enemies[23]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/16.f});

    enemies.clear();

    while (app.isOpen()) {
        static sf::Clock clock;

        sf::Event event;
        while (app.pollEvent(event)) {
            switch (event.type) {

            case sf::Event::Closed:
                app.close();
                break;

            case sf::Event::KeyReleased:

                switch (event.key.code) {

                case sf::Keyboard::Escape:
                    app.close();
                    break;

                case sf::Keyboard::W:
                    player.stop_move(Player::Direction::UP);
                    break;

                case sf::Keyboard::A:
                    player.stop_move(Player::Direction::LEFT);
                    break;

                case sf::Keyboard::S:
                    player.stop_move(Player::Direction::DOWN);
                    break;

                case sf::Keyboard::D:
                    player.stop_move(Player::Direction::RIGHT);
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::KeyPressed:

                switch (event.key.code) {

                case sf::Keyboard::W:
                    player.start_move(Player::Direction::UP);
                    break;

                case sf::Keyboard::A:
                    player.start_move(Player::Direction::LEFT);
                    break;

                case sf::Keyboard::S:
                    player.start_move(Player::Direction::DOWN);
                    break;

                case sf::Keyboard::D:
                    player.start_move(Player::Direction::RIGHT);
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::MouseButtonReleased:
                gun.reload();
                break;

            default:
                break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto target = sf::Mouse::getPosition(app);
            gun.fire(sf::Vector2f(target));
        }

        /* static int last_projectile_count = 0; */
        /* int projectile_count = gun.get_magazine().size(); */
        /* if (last_projectile_count != projectile_count) { */
        /*     last_projectile_count = projectile_count; */
        /*     std::cout << "Projectile Count: " << projectile_count << std::endl; */
        /* } */

        Game::instance().refresh_map();
        auto frame_length = clock.restart();

        for(auto& entity : game.entities()) {
            entity->refresh(frame_length);

            auto remaining = frame_length;
            int loops = 2;
            bool did_portal = false;
            while(remaining > sf::Time::Zero && loops--)
            {
                auto used = entity->update(remaining);
                remaining -= used;

                // allow one extra retry for a "portal" move
                // ie. entity is trying to unstick itself from something
                if (used == sf::Time::Zero && !did_portal) {
                    did_portal = true;
                    ++loops;
                }
            }

            entity->flush();
        }

        /* int entity_count = game.entities().size(); */

        auto& collection = game.entity_collection();
        collection.erase(
            std::remove_if(collection.begin(), collection.end(),
                           [](auto& entity) { return entity->is_dead(); }),
            collection.end());

        /* int delta_entities = entity_count - collection.size(); */
        /* if (delta_entities != 0) { */
        /*     std::cout << */
        /*         "Size: " << */
        /*         entity_count - delta_entities << " " << */
        /*         "Delta: " << */
        /*         delta_entities << */
        /*         std::endl; */
        /* } */

        Graphics::Renderings all_renderings;

        for(auto& entity : game.entities()) {
            if (entity->has_component<Graphics>()) {
                add_renderings(all_renderings, entity->get_component<Graphics>()->render());
            }
        }

        app.clear(sf::Color::White);

        for(const auto* rendering : all_renderings) {
            app.draw(*rendering);
        }

        auto map = Game::instance().get_map(&enemy_example);

        sf::RectangleShape impassable_tile;
        impassable_tile.setFillColor(sf::Color::Red);
        impassable_tile.setSize(Game::instance().get_tile_dimensions());
        impassable_tile.setOutlineColor(sf::Color::Black);
        impassable_tile.setOutlineThickness(-1.f);

        //draw tile map
        sf::Vector2i position = {0, 0};
        for(const auto& row : map) {
            position.x = 0;
            for(const auto& tile : row) {
                if (!tile.passable) {
                    impassable_tile.setPosition(Game::instance().get_position_for(position));
                    app.draw(impassable_tile);
                }
                ++position.x;
            }
        ++position.y;
        }

        // draw tile path
        sf::RectangleShape path_tile;
        path_tile.setFillColor(sf::Color::Blue);
        path_tile.setSize(Game::instance().get_tile_dimensions());
        path_tile.setOutlineColor(sf::Color::Black);
        path_tile.setOutlineThickness(-1.f);

        for(const auto& tile : enemy_example.get_path()) {
            path_tile.setPosition(Game::instance().get_position_for(tile));
            app.draw(path_tile);
        }

        frame_rate.setString("FrameRate: " + std::to_string(1.f/frame_length.asSeconds()));
        app.draw(frame_rate);

        timer.setString("Timer: " + std::to_string(1.f/Game::instance().get_timer().asSeconds()));
        app.draw(timer);

        app.display();
    }
    return 0;
}
