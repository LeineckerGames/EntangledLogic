The Global Audio Subsystem is used to play the background music within the game world. Currently there are three songs in the game and one ambience track that plays. The music and all other sound effects are handled using [Unreal's Meta Sounds]([# MetaSounds](https://dev.epicgames.com/documentation/unreal-engine/metasounds-in-unreal-engine)) which allows for a lot more features when creating and using audio in game.

## Music
***
At the beginning of loading the game world a background ambience track begins to play within the world. After a random amount of time the ambience track will fade into a randomly selected song to play from.

The random minimum and maximum amount of time can be modified within the [[Developer Settings]]. The selection of songs that are randomly chosen can be modified within the Background Music [Meta Sound]([# MetaSounds](https://dev.epicgames.com/documentation/unreal-engine/metasounds-in-unreal-engine)).

>[!warning] Loading Audio Assets
>Since Unreal's Subsystems are created in C++ it means that there is no way to load assets in the editor to be used within the Subsystem. Because of this we are using our custom [[Developer Settings]] to easily assign and load assets. Currently the Background Music Meta Sound is being loaded through the [[Player Camera Controller]]. This is a [[Known Issues & Bugs|Known Issue]] and should be moved to be loaded synchronously and through the [[Developer Settings]].

## Meta Sounds
***
