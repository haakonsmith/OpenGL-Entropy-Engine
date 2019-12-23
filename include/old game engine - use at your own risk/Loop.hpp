room->transform("__all", 100, glm::vec3(10, 0.0f, 0.0f));
thingX = my_time + cos(player->rotation) + SCREEN_WIDTH;
thingY = my_time + sin(player->rotation) + SCREEN_HEIGHT;
room->transform("tri", 0, glm::vec3(200, 200, 0.5f));
room->draw();

player->transform(player->rotation-47, glm::vec3(player->x, player->y, 1), glm::vec3(0.4f, 0.4f, 0.5f));
player->draw();
player->drawBullets();

triangle2->transform(my_time * 100, glm::vec3(my_time, 0.0f, 0.0f));
triangle2->draw();