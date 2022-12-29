# SogasEngine

Personal project between two friends to learn how to develop a game engine from scratch, using all sources we had available.

The engine is written in C++ and uses OpenGL, ImGUI and GLFW. It features three projects: The Engine itself, the Editor (runs on top of the engine) and the application (game being developed).

 <img src="https://github.com/AlvarAlonso/SogasEngine/blob/main/images/editor.PNG" alt="editor"/>

## Features

<ul>
  <li>
    Editor using ImGUI with dock.
  </li>
  <li>
    Entity Component System.
  </li>
  <li>
    Event System.
  </li>
  <li>
    Render abstraction for different APIs. Using only OpenGL right now.
  </li>
  <li>
    Scene Graph to feed to the renderer.
  </li>
  <li>
    Lua binding being able to attach scripts as entity components.
  </li>
  <li>
    Serialization and deserialization of scenes.
  </li>
  <li>
    Logger.
  </li>
</ul>
