#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Model3D.h"
#include "ECS/Actor.h"
#include "EngineUtilities\GUI/GUI.h"
#include "SceneGraph\SceneGraph.h"
#include "EngineUtilities\Utilities\Camera.h"
#include "EngineUtilities\Utilities\Skybox.h"
#include "EngineUtilities\Utilities\LayoutBuilder.h"
#include "EngineUtilities/Utilities/EditorViewportPass.h"

// Manejador externo para la integración de ImGui con Win32
extern IMGUI_IMPL_API
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * @class BaseApp
 * @brief Clase principal que orquesta el ciclo de vida del motor y la aplicación.
 * * BaseApp se encarga de inicializar la ventana, el dispositivo gráfico, los recursos
 * de renderizado (SwapChain, RenderTargets) y gestionar el bucle principal (Update/Render).
 * También integra el SceneGraph, la GUI y el manejo del Viewport para el editor.
 */
class
	BaseApp {
public:
	/**
	 * @brief Constructor por defecto.
	 */
	BaseApp() = default;

	/**
	 * @brief Destructor que asegura la limpieza de recursos de GPU y memoria.
	 */
	~BaseApp() { destroy(); }

	/**
	 * @brief Fase de "despertar" del motor.
	 * Se usa para configuraciones preliminares antes de la inicialización completa.
	 * @return S_OK si la fase se completó correctamente.
	 */
	HRESULT
		awake();

	/**
	 * @brief Inicia el bucle de mensajes de Windows y la ejecución del motor.
	 * @param hInst Instancia de la aplicación.
	 * @param nCmdShow Comando de visualización de la ventana.
	 * @return Código de salida de la aplicación.
	 */
	int
		run(HINSTANCE hInst, int nCmdShow);

	/**
	 * @brief Inicializa todos los componentes de DirectX y recursos de la escena.
	 * @return S_OK si todo el pipeline gráfico se configuró con éxito.
	 */
	HRESULT
		init();

	/**
	 * @brief Actualiza la lógica de todos los sistemas por frame.
	 * @param deltaTime Tiempo transcurrido entre frames.
	 */
	void
		update(float deltaTime);

	/**
	 * @brief Ejecuta el pipeline de renderizado completo.
	 */
	void
		render();

	/**
	 * @brief Libera todos los objetos de DirectX, memoria dinámica y cierra sistemas.
	 */
	void
		destroy();

	/**
	 * @brief Callback para manejar cambios de tamaño de la ventana principal.
	 * @param newW Nuevo ancho en píxeles.
	 * @param newH Nuevo alto en píxeles.
	 */
	void
		onResize(UINT newW, UINT newH);

	/**
	 * @brief Gestiona el redimensionado específico del área de renderizado del editor (Viewport).
	 * Útil para ajustar la resolución interna cuando se mueven paneles en la UI.
	 */
	void handleEditorViewportResize();

private:
	/**
	 * @brief Procedimiento de ventana (Window Procedure) para procesar eventos de Windows.
	 */
	static LRESULT CALLBACK
		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	// --- Core Gráfico ---
	Window                              m_window;           ///< Ventana nativa de Win32.
	Device                              m_device;           ///< Interfaz del dispositivo lógico (GPU).
	DeviceContext                       m_deviceContext;    ///< Contexto para enviar comandos de renderizado.
	SwapChain                           m_swapChain;        ///< Cadena de intercambio de buffers.
	Texture                             m_backBuffer;       ///< Textura que representa el buffer trasero.
	RenderTargetView                    m_renderTargetView; ///< Vista para escribir colores en el backbuffer.
	Texture                             m_depthStencil;     ///< Textura para el buffer de profundidad.
	DepthStencilView                    m_depthStencilView; ///< Vista para pruebas de profundidad y stencil.
	Viewport                            m_viewport;         ///< Definición del área de dibujo en pantalla.
	ShaderProgram                       m_shaderProgram;    ///< Shaders principales del motor.

	bool m_d3dReady = false;                                ///< Indica si DirectX se inicializó correctamente.
	Buffer m_constantBuffer;                                ///< Buffer constante de GPU.
	CBMain m_constantBufferStruct;                          ///< Estructura de datos vinculada al buffer constante.

	// --- Recursos de Texturizado (PBR/Standard) ---
	Texture m_AlbedoSRV;    ///< Textura de color base.
	Texture m_MetallicSRV;  ///< Mapa de metalicidad.
	Texture m_RoughnessSRV; ///< Mapa de rugosidad.
	Texture m_AOSRV;        ///< Mapa de oclusión ambiental.
	Texture m_NormalSRV;    ///< Mapa de normales.

	// --- Escena y Objetos ---
	Camera                              m_camera;           ///< Cámara principal para visualización.
	SceneGraph                          m_sceneGraph;       ///< Gestor de jerarquía de entidades.
	std::vector<EU::TSharedPointer<Actor>> m_actors;        ///< Lista de actores registrados.
	EU::TSharedPointer<Actor> m_cyberGun;                   ///< Referencia específica a un actor de prueba.
	Model3D* m_model;            ///< Puntero a un modelo 3D cargado.

	// --- Utilidades y GUI ---
	GUI                                 m_gui;              ///< Interfaz de usuario (ImGui).
	EU::Vector3 m_cameraPos;                                ///< Posición actual de la cámara en el mundo.
	Skybox m_skybox;                                        ///< Sistema de fondo y atmósfera.
	Texture                             m_skyboxTex;        ///< Textura cubemap para el skybox.
	RasterizerState m_defaultRasterizer;                    ///< Estado de rasterización por defecto.
	DepthStencilState m_defaultDepthStencil;                ///< Estado de profundidad por defecto.

	// --- Lógica del Viewport del Editor ---
	EditorViewportPass m_editorViewportPass;                ///< Pase de renderizado para el viewport del editor.
	bool m_editorViewportResizePending = false;             ///< Bandera para redimensionado diferido.
	unsigned int m_pendingViewportWidth = 1;                ///< Ancho pendiente de aplicar.
	unsigned int m_pendingViewportHeight = 1;               ///< Alto pendiente de aplicar.
	unsigned int m_lastRequestedViewportWidth = 1;          ///< Último ancho solicitado por la UI.
	unsigned int m_lastRequestedViewportHeight = 1;         ///< Última altura solicitada por la UI.
	int m_viewportResizeStableFrames = 0;                   ///< Contador para estabilizar el resize del viewport.
};