#pragma once
#include "Prerequisites.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include <imgui_internal.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

// Forward declarations para optimizar compilación
class Viewport;
class Window;
class Device;
class DeviceContext;
class Actor;
class Camera;

/**
 * @class GUI
 * @brief Sistema de Interfaz de Usuario para el Editor del Motor.
 * * Esta clase envuelve las funcionalidades de ImGui e ImGuizmo para proporcionar
 * paneles de control (Inspector, Outliner, Viewport) y herramientas de manipulación
 * de transformaciones en tiempo real dentro del motor.
 */
class
    GUI {
public:
    GUI() = default;
    ~GUI() = default;

    // --- Ciclo de Vida del Sistema de UI ---

    /**
     * @brief Configuración inicial de variables y estados de la UI.
     */
    void awake();

    /**
     * @brief Inicializa los contextos de ImGui para Win32 y DirectX 11.
     * @param window Referencia a la ventana nativa.
     * @param device Dispositivo DirectX para crear recursos de UI.
     * @param deviceContext Contexto para renderizar los comandos de ImGui.
     */
    void init(Window& window, Device& device, DeviceContext& deviceContext);

    /**
     * @brief Prepara el nuevo frame de la UI y gestiona la entrada de usuario.
     * @param viewport Referencia al viewport para cálculos de coordenadas.
     * @param window Referencia a la ventana para eventos de mouse/teclado.
     */
    void update(Viewport& viewport, Window& window);

    /**
     * @brief Finaliza el dibujado de ImGui y lo envía al pipeline de renderizado.
     */
    void render();

    /**
     * @brief Limpia los contextos de ImGui y libera memoria de texturas de la UI.
     */
    void destroy();

    // --- Acciones y Popups ---

    /**
     * @brief Dibuja la barra de herramientas principal en la parte superior.
     */
    void ToolBar();

    /**
     * @brief Muestra el diálogo de confirmación de salida.
     */
    void closeApp();

    /**
     * @brief Muestra información contextual al pasar el mouse sobre elementos.
     */
    void toolTipData();

    // --- Estilos Visuales ---

    /**
     * @brief Aplica un tema visual personalizado inspirado en estilos modernos.
     * @param opacity Nivel de transparencia de las ventanas.
     * @param accent Color de acento para botones y elementos activos.
     */
    void appleLiquidStyle(float opacity, ImVec4 accent);

    // --- Controles de Usuario Personalizados ---

    /**
     * @brief Dibuja un control especializado para editar vectores de 3 componentes (X, Y, Z).
     * @param label Etiqueta del control.
     * @param values Puntero al array de floats a modificar.
     * @param resetValues Valor al que regresará si se presiona el botón de reset.
     * @param columnWidth Ancho de la columna de etiquetas.
     */
    void
        vec3Control(const std::string& label,
            float* values,
            float resetValues = 0.0f,
            float columnWidth = 100.0f);

    // --- Paneles del Editor ---

    /**
     * @brief Muestra las propiedades básicas de un actor (Nombre, Transform, etc.).
     * @param actor El actor seleccionado.
     */
    void
        inspectorGeneral(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Contenedor secundario para componentes adicionales del actor.
     */
    void
        inspectorContainer(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Lista jerárquica de todos los actores presentes en la escena actual.
     * @param actors Vector de actores a listar.
     */
    void
        outliner(const std::vector<EU::TSharedPointer<Actor>>& actors);

    /**
     * @brief Dibuja la cinta superior (Ribbon) al estilo de los estudios modernos.
     */
    void
        drawStudioTopRibbon();

    /**
     * @brief Renderiza la textura del Viewport dentro de una ventana de ImGui.
     * @param viewportSRV Recurso de DirectX que contiene la escena renderizada.
     */
    void
        drawViewportPanel(ID3D11ShaderResourceView* viewportSRV);

    /**
     * @brief Habilita el sistema de ventanas anclables (Docking) para organizar el editor.
     */
    void
        drawEditorDockspace();

    // --- Transformacion y Gizmos ---

    /**
     * @brief Gestiona la manipulación visual (Translate, Rotate, Scale) del actor en el Viewport.
     * @param cam Cámara actual para calcular la proyección del Gizmo.
     * @param window Ventana para capturar clics en el Gizmo.
     * @param actor Actor cuyas matrices serán modificadas.
     */
    void
        editTransform(Camera& cam, Window& window,
            EU::TSharedPointer<Actor> actor);

    /**
     * @brief Dibuja la pequeña barra flotante para alternar entre Traslación, Rotación y Escala.
     */
    void
        drawGizmoToolbar();

    /**
     * @brief Convierte una XMMATRIX de DirectX a un array de floats compatible con ImGuizmo.
     * @param mat Matriz de entrada.
     * @param dest Puntero al array de destino (mínimo 16 floats).
     */
    void
        ToFloatArray(const XMMATRIX& mat, float* dest) {
        XMFLOAT4X4 temp;
        XMStoreFloat4x4(&temp, mat);
        memcpy(dest, &temp, sizeof(float) * 16);
    }

private:
    bool checkboxValue = true;
    bool checkboxValue2 = false;
    std::vector<const char*> m_objectsNames; ///< Nombres para el sistema de búsqueda/outliner.
    std::vector<const char*> m_tooltips;     ///< Mensajes de ayuda dinámicos.

    bool show_exit_popup = false;            ///< Control del estado del popup de salida.
    ImDrawList* m_viewportDrawList = nullptr;///< Lista de dibujo para superponer elementos en el Viewport.
    bool m_viewportActive = false;           ///< Indica si la ventana del viewport está abierta.

public:
    // Propiedades de estado del Viewport (Vitales para Input de Cámara y Gizmos)
    bool m_isUsingGizmo = false;      ///< @c true si el usuario está arrastrando un Gizmo actualmente.
    int selectedActorIndex = -1;      ///< Índice del actor seleccionado en el Outliner.
    ImVec2 m_viewportPos;             ///< Posición absoluta de la ventana del Viewport en pantalla.
    ImVec2 m_viewportSize;            ///< Dimensiones actuales del área de renderizado de la escena.
    bool m_viewportHovered = false;   ///< @c true si el mouse está sobre el Viewport.
    bool m_viewportFocused = false;   ///< @c true si la ventana del Viewport tiene el foco.
};