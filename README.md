# Syndicate Game Engine
Syndicate is a Game Engine written in C++, still in development.

A hobby project.

# Dependencies
- GLEW
- FreeImage
- FreeType
- FreeTypeGL
- Gorilla Audio (OpenAL)

# Usage
Clone the repository and set ***Venus*** as the startup project to see how it works.

## Creating Your Game

### Importing namespaces
I highly suggest that you import these namespaces, otherwise your code is going to be long!

```
using namespace Syndicate;
using namespace Maths;
using namespace Graphics;
```

### The Game class
The game class is the base class of your game. It must inherit the interface IGame and override the following methods:
```
// Initialize the game resources, layers, audio, fonts, etc here
bool Initialize() override;

// The is what the engine will call inside the game loop
bool Render() override;

// Called just before the engine shuts itself down.
// Deallocate any resources you allocated during Initialize here
bool Shutdown() override;
```

An example is provided insided the "Venus" project.

### Initializing the Engine

```
Engine engine(new Game());

engine.Initialize();
engine.Run();
engine.Shutdown();
```

### Creating your own layers
This is the first thing you want to do. A layer has a Renderer, currently Syndicate only has one renderer. I will implement more in the future. A layer also has a shader and a projection matrix.

#### Layer Structure:
- Renderer
- Shader
- Projection Matrix

The renderer and shader both must be allocated on the heap. These will be automatically deleted by Syndicate when the layer is destroyed.

```
class TestLayer : public Layer
{
public:
    TestLayer(Syndicate::Graphics::Renderer2D* renderer, Syndicate::Graphics::Shader* shader, Syndicate::Maths::Matrix4 projectionMatrix)
        : Layer(renderer, shader, projectionMatrix)
    {

    }

    ~TestLayer();
}
```

### Using your custom layer
```

// Somewhere inside the class header...

public:
    TestLayer m_Layer;
    Shader* m_Shader;

// and the class implementation...

// Initialize the layer
bool Game::Initialize()
{
    // Load your shaders
	m_Shader = synnew Shader("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag");

    // This will be fixed in a later version...
	int textureSlots[] = { 0,1,2,3,4,5,6,7,8,9 };

	m_Shader->Enable();
	m_Shader->setUniform2f("light_pos", Vector2(0.0, 0.0));
	m_Shader->setUniformMat4("pr_matrix", Matrix4::Orthographic(-16.0f, 16.0f, 9.0f, -9.0f, 1.0f, -1.0f));
	m_Shader->setUniform1iv("textures", textureSlots, 10);
	m_Shader->Disable();

	m_Layer.SetShader(m_Shader);
	m_Layer.SetRenderer(synnew BatchRenderer2D());
	m_Layer.SetProjectionMatrix();

	Group *logo = synnew Group(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	logo->Add(synnew Sprite(Vector3(-9.0f, 0, 0), Vector2(20, 1.5f), Maths::Vector4(26, 26, 26, 255)));
	m_Layer.Add(logo);

	return true;
}

bool Game::Render()
{
	m_Layer.Render();
	return true;
}

bool Game::Shutdown()
{
	m_Layer.Free();
	return true;
}
```

### Rendering Sprites
Once you have a layer, you can add sprites to it and they will be rendered. All sprites derive from Renderable2D and must be allocated on the heap. You can also create your custom Renderable2Ds.

#### Sprite Structure: 
- Position(x, y, z) (Maths::Vector3)
- Size(width, height) (Maths::Vector2) 
- Color(red, blue, green, alpha) (Color) OR path/to/texture

```
layer->Add(new Sprite(Vector3(0, 0, 0), Vector2(10, 10), Color(255, 0, 0, 255));
layer->Add(new Sprite(Vector3(-10, 5, 0), Vector2(10, 10), "path/to/my/texture.png");
```

### Groups of Sprites
Groups are also Renderable2Ds that can contain other Sprites. The children of a group have transformation matrices relative to the group.

#### Group Structure: 
- Transformation Matrix (Maths::Matrix4)

```
Group *group = new Group(Matrix4::Translation(Vector3(-16.0f, 7.0f, 0.0f)));
group->Add(layer->Add(new Sprite(Vector3(0, 0, 0), Vector2(10, 10), Color(255, 0, 0, 255)));
group->Add(layer->Add(new Sprite(Vector3(10, 0, 0), Vector2(10, 10), Color(0, 255, 0, 255)));
layer->Add(group);
```

### Font Rendering
Font rendering is pretty straigh forward. Fonts are rendered inside a ***Label*** which is just a Renderable2D and can be easily added to groups and layers.

FontManager is used to load fonts using the loadFont method which takes in two arguments, one for accessing the font via its name and the other for its path.

```
// First we set the scale relative to screen coordinates
Font::setScale(Window::i()->getWidth() / 16.0f, Window::i()->getHeight() / 9.0f);

// Load a font using the font manager
FontManager::loadFont("RalewayLight", "res/Fonts/Raleway/Raleway-Light.ttf");

Label* fps = new Label("0 fps", Vector2(0.5f, 0.35f), Font("RalewayLight", 60, Color(0xffffff)));
layer->Add(fps);

while (!window.Closed())
{
    layer->Render();
}

delete layer;
```

### Audio & Sounds
Playing audio is pretty simple in Syndicate. An Audio manager runs on a separate thread that manages all the sounds and audio files for you.

```
Syndicate::AudioManager::i()->Load( new Audio("AccessKey", "path/to/ogg/or/wav/file") );
Syndicate::AudioManager::i()->Get( "AccessKey" )->Play();
```

# TODO
- Implement an Input Manager
- An Event Manager
- ~~Remove GLFW and use Windows API for creating the OpenGL context~~ ***Done!***
- Implement a proper Texture Manager
- Implement DirectX
- and things I can't remember.... :)


# License

    MIT License

    Copyright (c) 2017 AmN

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
