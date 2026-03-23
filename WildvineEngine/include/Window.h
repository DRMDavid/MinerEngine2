#pragma once
#include "Prerequisites.h"

// Forward declaration para evitar dependencias circulares con la clase principal
class BaseApp;

/**
 * @class Window
 * @brief Encapsula la creación y gestión de una ventana nativa de Windows (Win32).
 * * Esta clase se encarga de registrar la clase de ventana, crear el handle (HWND)
 * y gestionar las dimensiones de la pantalla. Es el contenedor físico donde
 * el SwapChain de DirectX realizará el dibujado.
 */
class
	Window {
public:
	/**
	 * @brief Constructor por defecto.
	 */
	Window() = default;

	/**
	 * @brief Destructor por defecto.
	 */
	~Window() = default;

	/**
	 * @brief Registra e inicializa la ventana del sistema operativo.
	 * * @param hInstance Instancia de la aplicación proporcionada por el SO.
	 * @param nCmdShow Estado de visualización inicial (Minimizado, Maximizado, etc.).
	 * @param wndproc Puntero a la función encargada de procesar los mensajes de Windows.
	 * @param app Puntero a la aplicación base para vinculación de eventos.
	 * @return S_OK si la ventana se creó correctamente, o un código de error HRESULT.
	 */
	HRESULT
		init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc, BaseApp* app);

	/**
	 * @brief Procesa los eventos pendientes en la cola de mensajes de Windows.
	 */
	void
		update();

	/**
	 * @brief Placeholder para operaciones de renderizado ligadas a la ventana.
	 */
	void
		render();

	/**
	 * @brief Destruye el handle de la ventana y libera recursos del sistema.
	 */
	void
		destroy();

public:
	/**
	 * @brief Handle de la ventana de Windows (identificador nativo).
	 */
	HWND m_hWnd = nullptr;

	/**
	 * @brief Ancho actual del área cliente de la ventana en píxeles.
	 */
	unsigned int m_width;

	/**
	 * @brief Alto actual del área cliente de la ventana en píxeles.
	 */
	unsigned int m_height;

private:
	/**
	 * @brief Instancia de la aplicación vinculada a la ventana.
	 */
	HINSTANCE m_hInst = nullptr;

	/**
	 * @brief Estructura que almacena las coordenadas del rectángulo de la ventana.
	 */
	RECT m_rect;

	/**
	 * @brief Título de la ventana que aparecerá en la barra superior.
	 */
	std::string m_windowName = "MinerEngine";
};