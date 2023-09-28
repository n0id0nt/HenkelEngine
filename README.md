# Henkel Engine
This is intended to be a small lightweight 2d custom game engine build ontop of sdl2 and opengl. For making simple 2d games. 

my intention is that Tiled will be used as the level editor and scripts will be linked to objects via tiled. this is probably not the best way of doing things but i dont want to write a level editor and i also want to keep the level editor as seperated from the code as possible. 

The main structure of the game is:
- main
- Application 
- Window
- Scene
  - Camera
  - GameObjects
    - Transform
    - Components
    - ChildGameObjects

The the list all objects behavious are derived from components 
The component list is: (* means is still being implements)
- Sprite *
- TileMap *
- Script *
- Audio *
- Collision Body *
- Animation *
- Particles *
