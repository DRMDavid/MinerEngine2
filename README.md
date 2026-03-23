<p align="center">
  <img src="https://img.shields.io/badge/Direct3D-11-1155BA?style=for-the-badge&logo=windows&logoColor=white" alt="D3D11"/>
  <img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17"/>
  <img src="https://img.shields.io/badge/Win32-API-1f6feb?style=for-the-badge" alt="Win32 API"/>
  <img src="https://img.shields.io/badge/ECS-Architecture-orange?style=for-the-badge" alt="ECS"/>
  <img src="https://img.shields.io/badge/ImGui-Docking-8A2BE2?style=for-the-badge" alt="ImGui Docking"/>
</p>

<h1 align="center">⛏️ MinerEngine 2 (Wildvine Engine) – Motor Gráfico & ECS</h1>

---

## 📘 Resumen

**⛏️ MinerEngine 2** (impulsado por el núcleo *WildvineEngine*) es un robusto motor gráfico y de videojuegos desarrollado desde cero en C++ y **Direct3D 11**. 

Esta versión destaca por su actualización profunda de interfaz (**Cambio de GUI**) integrando las capacidades avanzadas de anclaje de *ImGui Docking*. Además, el motor implementa una arquitectura **Entity-Component-System (ECS)** acoplada a un **SceneGraph**, permitiendo una gestión modular y jerárquica de entidades (`Actors`). Cuenta con su propia biblioteca de utilidades (Math & Custom Memory), un `ResourceManager` eficiente, y soporte para la carga de modelos 3D complejos mediante el **FBX SDK**.

---

## 🧭 Índice

- [📘 Resumen](#-resumen)
- [✨ Características Principales](#-características-principales)
- [🏗️ Arquitectura del Motor](#️-arquitectura-del-motor)
  - [Core & Utilities](#core--utilities)
  - [Sistema ECS & SceneGraph](#sistema-ecs--scenegraph)
  - [Gráficos & Recursos](#gráficos--recursos)
- [🖥️ Tecnologías Integradas](#️-tecnologías-integradas)
- [🚀 Flujo de Ejecución](#-flujo-de-ejecución)
- [🧪 Requisitos y Construcción](#-requisitos-y-construcción)

---

## ✨ Características Principales

El motor ha evolucionado hacia una arquitectura de nivel profesional con las siguientes implementaciones:

| Característica | Descripción |
|---|---|
| **Arquitectura ECS** | Sistema de `Entity`, `Component` y `Actor` para desacoplar completamente la lógica de los datos. |
| **Scene Graph** | Jerarquía de nodos (`HierarchyComponent`) para gestionar relaciones padre-hijo y transformaciones relativas en el mundo. |
| **Advanced GUI (Docking)** | Interfaz gráfica inmediata renovada usando la rama *Docking* de ImGui, permitiendo layouts de editor profesionales y visualización de datos en tiempo real. |
| **Custom Memory** | Gestión de memoria propia mediante punteros inteligentes (`TSharedPointer`, `TWeakPointer`, `TUniquePtr`, `TStaticPtr`). |
| **Custom Containers** | Estructuras de datos dinámicas optimizadas: `TArray`, `TMap`, `TSet`, `TPair`. |
| **Math Library** | Librería matemática interna para álgebra lineal: `Vector2/3/4`, `Matrix2x2/3x3/4x4`, `Quaternion`. |
| **Asset Pipeline** | Carga de modelos 3D industriales (mallas, jerarquías) utilizando **FBX SDK** y parseo XML vía **libxml2**. |

---

## 🏗️ Arquitectura del Motor

### Core & Utilities
El núcleo del motor evita la dependencia excesiva de la STL estándar, favoreciendo implementaciones personalizadas que otorgan mayor control sobre la asignación de memoria y el rendimiento general.
* **Memory Management:** Garantiza la correcta liberación de recursos evitando *memory leaks*.
* **Editor Viewport:** Implementación de un `EditorViewportPass` para separar el renderizado del juego de la interfaz del editor.

### Sistema ECS & SceneGraph
La lógica del juego se estructura mediante composición y jerarquía:

| Clase principal | Responsabilidad |
|---|---|
| **Actor** | Entidad base que existe en el mundo. Contiene un identificador único y una lista dinámica de componentes. |
| **Component** | Interfaz base para dotar de comportamiento y datos a los actores. |
| **Transform** | Define posición, rotación (`Quaternion`) y escala en el espacio 3D. |
| **HierarchyComponent** | Conecta actores dentro del `SceneGraph`, propagando transformaciones de padres a hijos. |
| **MeshComponent** | Enlaza la geometría (`Model3D`) y los materiales con el `Actor` para su paso por el pipeline gráfico. |

### Gráficos & Recursos
* **ResourceManager:** Patrón Singleton encargado de cargar, almacenar en caché y proveer recursos (`IResource`, Texturas, Shaders) para evitar duplicidad en la VRAM.
* **Renderer Pipeline:** Abstracción completa de D3D11 gestionando `Device`, `DeviceContext`, `SwapChain`, `RenderTargetView`, `DepthStencilState` y buffers (`Buffer.h`).

---

## 🖥️ Tecnologías Integradas

| Tecnología / Librería | Propósito en el Motor |
|---|---|
| **Direct3D 11** | API Gráfica principal para el renderizado acelerado por hardware. |
| **Win32 API** | Creación del contexto de ventana (`Window.h`) y bucle de mensajes (WndProc). |
| **Dear ImGui (Docking)** | Sistema de UI para la creación de las herramientas del editor y paneles *dockeables*. |
| **FBX SDK** | Importación de assets 3D estándar de la industria (.fbx). |
| **STB Image** | Decodificación y carga ligera de texturas (`stb_image.h`). |
| **libxml2** | Lectura y parseo de archivos de configuración y datos estructurados en XML. |

---

## 🚀 Flujo de Ejecución

1. **Inicialización (`BaseApp` / `WildvineEngine.cpp`):**
   * Registro y creación de la ventana Win32.
   * Inicialización del contexto gráfico de Direct3D 11 (`Device`, `SwapChain`).
   * Configuración del backend de **ImGui** (Win32 + DX11) con soporte para múltiples viewports.
2. **Game Loop:**
   * **Input:** Captura y procesamiento de eventos del sistema operativo.
   * **Update:** Recorrido del `SceneGraph` y actualización lógica de los `Actors` y sus `Components` (cálculo de matrices de mundo).
   * **Render:**
     * Limpieza de los *Render Targets* y el *Depth Stencil*.
     * Configuración de *Shaders*, *Rasterizer States* y *Samplers*.
     * Dibujado de geometría (Pase del Editor / Pase del Juego).
     * Dibujado de la interfaz de usuario (ImGui Render Data).
     * Intercambio de buffers (`SwapChain::Present()`).
3. **Shutdown:**
   * Liberación segura de recursos COM de DirectX.
   * Destrucción de contextos de ImGui.
   * Limpieza de memoria general a través del gestor de contenedores propios.

---

## 🧪 Requisitos y Construcción

| Requisito | Detalle |
|---|---|
| **IDE** | Visual Studio 2019 o 2022 (Soporte para C++17) |
| **SDKs** | DirectX SDK (Windows Kit), FBX SDK 2020.0+ |
| **Arquitectura** | x64 (Recomendado para Debug y Release) |

**Pasos para compilar:**
1. Clona este repositorio (rama `CambioDeGUI`).
2. Asegúrate de tener instalado el **FBX SDK**. Debes configurar las macros o rutas de los directorios `Include` y `Lib` en las propiedades del proyecto (`WildvineEngine_2010.vcxproj` o `.sln`).
3. Abre la solución en Visual Studio.
4. Selecciona la configuración `Release` o `Debug` en arquitectura `x64`.
5. Compila la solución (`Ctrl + Shift + B`) y ejecuta.
