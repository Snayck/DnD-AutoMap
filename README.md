# Dark and Darker Automatic Map
#### Automatically detects what map you're currently playing on and displays it.
![alt text](/image.png?raw=true)
## Credits / Dependencies
<p> If you plan on using bits of the code or reference it in your own please credit me in your project </p>
<p> Uses some of the logic behind https://github.com/mtHuberty/dndgps. </p>
<p> https://github.com/ozymandiaslone/Darkographer might be better but I don't know 'go' so I wrote my own in c++ </p>

#### Requires: vcpkg
- vcpkg will acquire https://github.com/opencv/opencv on build

 ## to do
- I plan on adding automatic map icons for chests, bosses, revive tables, etc.
  - Might rip json for marker locations from https://darkanddarker.map.spellsandguns.com/ or i might directly integrate a browser window for it.

## Concerns
As to not flag the AntiCheat in any way I chose to take screenshots of the minimap rather than just reading the memory for what map you're currently playing. I also don't plan on making it into any sort of overlay and want to keep it seperate as the AntiCheat takes full screenshots of all monitors. Having any sort of overlay might seem like a cheat client showing the map in screenshots. I originally planned on using ImGui to display the images chose against it after realizing how much of a pain it is to read pngs into ImTextureID without using any sort of external library.
