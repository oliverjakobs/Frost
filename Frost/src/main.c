#include "Application/Application.h"

#include "SceneManager.h"

#include "gui/gui.h"

#include "Framebuffer.h"

SceneManager scene_manager;
Camera camera;

FrameBuffer framebuffer;
IgnisShader framebuffer_shader;

void OnEvent(Application* app, const Event e)
{
	if (e.type == EVENT_WINDOW_RESIZE)
	{
		ApplicationSetViewport(app, 0, 0, e.window.width, e.window.height);
	}

	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
		{
		case KEY_ESCAPE:
			ApplicationClose(app);
			break;
		case KEY_F5:
			ApplicationPause(app);
			break;
		case KEY_F6:
			ApplicationToggleVsync(app);
			break;
		case KEY_F7:
			ApplicationToggleDebugMode(app);
			break;
		case KEY_F8:
			ApplicationToggleGui(app);
			break;
		case KEY_1:
			SceneManagerChangeScene(&scene_manager, "scene");
			break;
		case KEY_2:
			SceneManagerChangeScene(&scene_manager, "scene2");
			break;
		}
	}

	SceneManagerOnEvent(&scene_manager, e);
}

void OnUpdate(Application* app, float deltaTime)
{
	/* discard frames that took to long */
	// if (deltaTime > 0.4f) return;

	SceneManagerOnUpdate(&scene_manager, deltaTime);
}

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Ignis/Packages/stb_image_write.h"

void OnRender(Application* app)
{
	FrameBufferBind(&framebuffer);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now

	SceneManagerOnRender(&scene_manager);

	/*
	GLubyte* data = (GLubyte*)malloc(4 * framebuffer.width * framebuffer.height);

	if (data)
	{
		glReadPixels(0, 0, framebuffer.width, framebuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_flip_vertically_on_write(1);

		stbi_write_bmp("output.bmp", framebuffer.width, framebuffer.height, 4, data);
	}

	free(data);
	*/

	FrameBufferUnbind();

	BatchRenderer2DStart(ApplicationGetScreenProjFlipPtr(app));

	BatchRenderer2DRenderTexture(&framebuffer.texture, 0.0f, 0.0f, app->width, app->height);

	BatchRenderer2DFlush();
}

void OnRenderDebug(Application* app)
{
	SceneManagerOnRenderDebug(&scene_manager);
}

void OnRenderGui(Application* app)
{
	gui_start();

	/* fps */
	if (gui_begin_align(GUI_HALIGN_LEFT, GUI_VALIGN_TOP, 200.0f, 28.0f, 8.0f, GUI_BG_NONE))
	{
		gui_text("FPS: %d", app->timer.fps);
	}
	gui_end();

	/* Debug */
	if (gui_begin_align(GUI_HALIGN_RIGHT, GUI_VALIGN_BOTTOM, 250.0f, 105.0f, 8.0f, GUI_BG_FILL))
	{
		gui_text("Scene: %s", scene_manager.scene_name);
		gui_separator();
		EcsEntity* player = SceneGetEntity(scene_manager.scene, "player", 1);
		if (player)
		{
			gui_text("Name: %s", player->name);
			vec2 position = EcsEntityGetPosition(player);
			gui_text("Position: %4.2f, %4.2f", position.x, position.y);
			gui_text("Precise Y: %f", position.y);
		}
	}
	gui_end();

	/* Settings */
	if (gui_begin_align(GUI_HALIGN_RIGHT, GUI_VALIGN_TOP, 220.0f, 140.0f, 8.0f, GUI_BG_FILL))
	{
		gui_text("F1: Toggle edit mode");
		gui_text("F5: Pause/Unpause");
		gui_text("F6: Toggle Vsync");
		gui_text("F7: Toggle debug mode");
		gui_text("F8: Toggle Gui");
		gui_text("DEL: Remove Trees");
	}
	gui_end();

	if (scene_manager.editmode)
	{
		if (gui_begin_align(GUI_HALIGN_LEFT, GUI_VALIGN_BOTTOM, 220.0f, 140.0f, 8.0f, GUI_BG_FILL))
		{
			gui_text("Hovered Entity: %s", scene_manager.hover ? scene_manager.hover->name : "null");
			gui_checkbox("Show grid", &scene_manager.showgrid);

			gui_separator();

			for (size_t i = 0; i < scene_manager.scene->max_layer; i++)
			{
				if (scene_manager.scene->layers[i].size > 0)
				{
					char buffer[16];
					snprintf(buffer, sizeof(buffer), "Layer: %zu", i);
					// ImGui::RadioButton(buffer, &scene_manager.layer, (int)i);
				}
			}
		}
		gui_end();
	}
	
	SceneManagerOnRenderGui(&scene_manager);

	gui_render(ApplicationGetScreenProjPtr(app));
}

int main()
{
	Application* app = (Application*)malloc(sizeof(Application));
	ApplicationLoadConfig(app, "config.json");

	/* ---------------| Config |------------------------------------------ */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	Renderer2DInit("res/shaders/renderer2D.vert", "res/shaders/renderer2D.frag");
	Primitives2DInit("res/shaders/primitives.vert", "res/shaders/primitives.frag");
	BatchRenderer2DInit("res/shaders/batchrenderer.vert", "res/shaders/batchrenderer.frag");
	FontRendererInit("res/shaders/font.vert", "res/shaders/font.frag");

	FontRendererBindFont(ResourceManagerGetFont(&app->resources, "font"), IGNIS_WHITE);

	/* framebuffer */
	FrameBufferGenerate(&framebuffer, app->width, app->height);

	const char* vert = \
		"#version 330\n"
		"layout(location = 0) in vec2 a_Position;\n"
		"layout(location = 1) in vec2 a_TexCoord;\n"
		"out vec2 v_TexCoord;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(a_Position, 0.0f, 1.0f);\n"
		"	v_TexCoord = a_TexCoord;\n"
		"}\n";

	const char* frag = \
		"#version 330\n"
		"layout(location = 0) out vec4 f_Color;\n"
		"in vec2 v_TexCoord;\n"
		"uniform sampler2D u_Texture;\n"
		"void main()\n"
		"{\n"
		"	f_Color = texture(u_Texture, v_TexCoord);\n"
		"}\n";

	ignisShaderSrcvf(&framebuffer_shader, vert, frag);
	ignisSetUniform1i(&framebuffer_shader, "u_Texture", 0);

	gui_init((float)app->width, (float)app->height);
	gui_set_font(ResourceManagerGetFont(&app->resources, "gui"), IGNIS_WHITE);

	ApplicationEnableDebugMode(app, true);
	ApplicationEnableVsync(app, false);
	ApplicationShowGui(app, true);

	CameraCreateOrtho(&camera, (vec3){ app->width / 2.0f, app->height / 2.0f, 0.0f }, (vec2){ (float)app->width, (float)app->height });
	SceneManagerInit(&scene_manager, &app->resources, &camera, 32.0f, 4);
	SceneManagerRegisterScenes(&scene_manager, "res/templates/scenes/register.json");
	SceneManagerChangeScene(&scene_manager, "scene");

	ApplicationSetOnEventCallback(app, OnEvent);
	ApplicationSetOnUpdateCallback(app, OnUpdate);
	ApplicationSetOnRenderCallback(app, OnRender);
	ApplicationSetOnRenderDebugCallback(app, OnRenderDebug);
	ApplicationSetOnRenderGuiCallback(app, OnRenderGui);

	ApplicationRun(app);

	FrameBufferDelete(&framebuffer);

	gui_free();

	FontRendererDestroy();
	Primitives2DDestroy();
	BatchRenderer2DDestroy();
	Renderer2DDestroy();

	ApplicationDestroy(app);

	free(app);

	return 0;
}