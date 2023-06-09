<!-- ABOUT THE PROJECT -->
#  Awesome 2D Shooter Game

<!-- A screenshot of your game would be nice here... -->

This is a short and succint description of this project. 
Zombie shooter, shoot zombies with pizza to make them not hungry enough to eat your brain and win, shoot as many zombies as possible in the set timeframe to meet a set score.
:running_man:

Features:
* Use weapon tilt to attack as many enemies as possible :bomb:
* Shoot as many zombies as possible within the timer
* Tilemaps loaded from notepad file
* Collect as many points as possible from the zombies killed within the timeframe
* Enemies respawn
* Display scores

## Dependencies
* SDL library
* SDL_image library
* SDL_ttf library
* SDL_mixer library

## Target platforms
Support Windows x86/x64, GNU/Linux, OSX.

Instructions to build on Windows platforms:
1) Load .sln solution with Visual Studio;
2) Select Release configuration, and x64 Architecture;
3) Compile and run the project.

## Code profiling
_Test date **29/01/2022**_;
Code profile for CPU, code was being ran for 1 minute and 9 seconds. The highest CPU usage was at the beginning when all the code was being initated, with a CPU usage of 15%. After the ititalizing, the code stayed on average between 4-0%; these higher uses of the CPU would be from using bullets and movement (IO). As you can see from the graph, 39.3% of the CPU usage was from IO which is bullets and movement, 32.7% of CPU usage is graphics, e.g. player and tilemap sprites, 23.6% of usage is kernel so the actual window of the game, UI is low (2.2%) as the UI is only a line of text right now with no use, and other which is 1.6%.
![image](https://user-images.githubusercontent.com/113985493/204526272-89ce3b62-7872-47ba-974d-137381dc6cb5.png)
Here we can see the individual causes for CPU usage, GameLoop and FontRenderer - for example - are taking a lot of usage
![image](https://user-images.githubusercontent.com/113985493/204528817-070dd296-51ad-4450-9581-70389745b526.png)

For memory usage, the usage is very consistent in the game no matter if using arrow keys or shooting mechanics. The memeory usage ranges from 22.9-21.6 MB, it would benefit from a decrease in memory usage however. Memory use increased as the game was being initiated.
![image](https://user-images.githubusercontent.com/113985493/204530853-edb75f04-fb4f-47f2-b5f8-7148c05d4d29.png)


_Test date **31/01/2022**_


CPU usage, 99.6% of the graph is kernel, which is just the main window of the game, since we cannot change much about the game window, we will check the other 0.4% of the game inside functions.
![Screenshot_20221230_173522](https://user-images.githubusercontent.com/113985493/210150030-7b61ab0f-ec39-4414-960c-d9d458a4faf8.png)
As we can see here, aside from the main running of the game, gameloop and font renderer renderers take up the most CPU use of 46-48%, which means that the rendering of sprites and fonts for time and score may need some changing; if we change these things, it will reduce CPU usage as the game main window consists of evrything running inside the game overall - reduce the biggest runners of the game and main CPU usage will also decrease
![Screenshot_20221230_180232](https://user-images.githubusercontent.com/113985493/210150103-bf851ded-e1d2-4fae-84b8-c8f64950948d.png)

Memory management however appears to have a leak that is at a constant rise of a few MB a second.
![Screenshot_20221231_154529](https://user-images.githubusercontent.com/113985493/210149897-e832f892-2ad4-4fb7-a264-128d9d463c41.png)


## Code optimization
After investigation into memory management, I discovered the leak was caused by audio never being cleaned; after a thourough test including tests from game win, game over, movement, collisions and sound effects via shooting, I have now discovered a change in memory usage where after the first game over/game win is complete, memory stays consistently at 45MB and no higher, which completes the leak problem.

![image](https://user-images.githubusercontent.com/113985493/210150011-2d51218e-d7a5-42ec-ae5f-d427a7645a69.png)
![image](https://user-images.githubusercontent.com/113985493/210150005-035b5d72-c2bc-404d-b262-60f692a021e8.png)
Result:
![image](https://user-images.githubusercontent.com/113985493/210149883-2e05bd89-3995-441d-b0f4-c3c1eea6ab62.png)

Now, since I have multiple enemies spawning in, it is a highly educated guess that CPU usage will decrease if code optimization is done in the rendering of enemies. Before optimization, the enemies would both have their own spawning code (one for enemy1 and one for enemy2), I have combined the code so the enemies both spawn and the code is only used once rather than done twice via using a struct, which saves CPU usage; I have also removed uneeded variables.
![image](https://user-images.githubusercontent.com/113985493/210157609-7593b89f-61de-4332-aa0d-86ecd722efbe.png)
![image](https://user-images.githubusercontent.com/113985493/210157616-a187ad1b-c5a4-4289-bba3-e5bfe7f34a62.png)
![image](https://user-images.githubusercontent.com/113985493/210157670-17a2814e-a97b-4d03-88ff-738081a217c8.png)
Enemy.cpp:
![image](https://user-images.githubusercontent.com/113985493/210157624-7a90f3ae-b03f-4aab-9880-8022d0d73391.png)
![image](https://user-images.githubusercontent.com/113985493/210157628-d1265e20-5b85-4283-aafc-ec91969d4d1d.png)
![image](https://user-images.githubusercontent.com/113985493/210157632-c3c46edf-fb5a-4198-8209-571fbd02186c.png)
![image](https://user-images.githubusercontent.com/113985493/210157639-722c761a-51dd-4b02-9f4b-ea0eb2b9549d.png)
![image](https://user-images.githubusercontent.com/113985493/210157646-bc89b3eb-a2f6-4a1c-91d9-287cb132fceb.png)
Now that the enemies only use one vector to hold both enemy data, it makes the code more efficient and easier to add features in the future.

After the renderer changes, we can see an outstanding change in results inside our final code profiler for CPU usage, which I am incredibly impressed with; the beginning of the game starts at 7% to start everything up, which may be 1% larger than last time, however now after the game is set up, the new optimised code now has a CPU usage of 0-2% which is by far the smallest CPU usage yet!
![image](https://user-images.githubusercontent.com/113985493/210157849-c0feec79-a4dd-4346-b43b-1bda6899eea2.png)
We can also see a dramatic decrease in percentage usage between the functions, both from gameloop renderer and font renderer renderer, which was the aim of the code optimization.
![image](https://user-images.githubusercontent.com/113985493/210157962-da4ff85d-3c54-46f3-a6f5-d2e3899d68a8.png)

In conclusion, the code optimization was a huge success!


## Credits
Background music -> Art Of Silence by Uniq : https://www.chosic.com/download-audio/25944/
Sound effect -> Sound Effect by <a href="https://pixabay.com/users/666herohero-25759907/?utm_source=link-attribution&amp;utm_medium=referral&amp;utm_campaign=music&amp;utm_content=21834">666HeroHero</a> from <a href="https://pixabay.com/sound-effects//?utm_source=link-attribution&amp;utm_medium=referral&amp;utm_campaign=music&amp;utm_content=21834">Pixabay</a>

## Additional information
Name - Chloe Woodman
Student id - 22194101
Lab machine was the 2nd closest row to the door - inner middle area, second computer from the front in the Games lab, MP135

## Contact

Chloe Woodman - chloewoodman03xo@gmail.com
