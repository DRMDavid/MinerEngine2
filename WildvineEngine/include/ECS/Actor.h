#pragma once
#include "Prerequisites.h"
#include "Entity.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"
#include "SamplerState.h"
#include "RasterizerState.h"
//#include "BlendState.h"
#include "ShaderProgram.h"
#include "DepthStencilState.h"

// Forward declarations para optimizar tiempos de compilación
class Device;
class DeviceContext;
class MeshComponent;

/**
 * @class Actor
 * @brief Representa una entidad gráfica con mallas, texturas y estados de renderizado.
 *
 * Un Actor es una entidad del motor que contiene mallas, texturas y recursos de renderizado
 * necesarios para dibujar un objeto en la escena. Administra buffers de vértices e índices,
 * estados de rasterización, blending y shaders, además de soportar renderizado de sombras.
 */
class
	Actor : public Entity {
public:
	/**
	 * @brief Constructor por defecto.
	 */
	Actor() = default;

	/**
	 * @brief Constructor que inicializa el actor con un dispositivo.
	 * @param device Referencia al dispositivo usado para inicializar recursos de hardware.
	 */
	Actor(Device& device);

	/**
	 * @brief Destructor virtual por defecto.
	 * Asegura la correcta liberación de memoria en clases derivadas.
	 */
	virtual
		~Actor() = default;

	/**
	 * @brief Se llama cuando el objeto es "despertado" en la escena.
	 * Útil para lógica que debe ocurrir antes del primer update.
	 */
	void
		awake() override {}

	/**
	 * @brief Inicializa el actor.
	 * Método heredado de @c Entity. Puede usarse para configurar recursos adicionales.
	 */
	void
		init() override {}

	/**
	 * @brief Actualiza la lógica del actor en cada frame.
	 * * @param deltaTime Tiempo transcurrido (en segundos) desde el último frame.
	 * @param deviceContext Contexto del dispositivo para operaciones de actualización de buffers.
	 */
	void
		update(float deltaTime, DeviceContext& deviceContext) override;

	/**
	 * @brief Realiza el dibujado estándar del actor en la escena.
	 * Configura estados de render, buffers y shaders antes de realizar las llamadas de dibujo.
	 * * @param deviceContext Contexto del dispositivo encargado de enviar comandos a la GPU.
	 */
	void
		render(DeviceContext& deviceContext) override;

	/**
	 * @brief Renderizado especializado para el Skybox.
	 * Ajusta los estados de profundidad y shaders específicos para el fondo de la escena.
	 * * @param deviceContext Contexto del dispositivo.
	 */
	void
		renderForSkybox(DeviceContext& deviceContext);

	/**
	 * @brief Libera todos los recursos de GPU asociados al actor.
	 * Limpia buffers, estados, shaders y texturas de la memoria de video.
	 */
	void
		destroy();

	/**
	 * @brief Establece y prepara las mallas del actor.
	 * Inicializa automáticamente los Vertex e Index Buffers basados en las mallas proporcionadas.
	 * * @param device Dispositivo con el cual se crean los buffers de hardware.
	 * @param meshes Vector de componentes de malla (MeshComponent).
	 */
	void
		setMesh(Device& device, std::vector<MeshComponent> meshes);

	/**
	 * @brief Obtiene el nombre identificativo del actor.
	 * @return Un string con el nombre del actor.
	 */
	std::string
		getName() { return m_name; }

	/**
	 * @brief Establece un nombre para el actor.
	 * @param name Cadena de texto con el nuevo nombre.
	 */
	void
		setName(const std::string& name) { m_name = name; }

	/**
	 * @brief Asigna un conjunto de texturas al actor para su renderizado.
	 * @param textures Vector de objetos @c Texture.
	 */
	void
		setTextures(std::vector<Texture> textures) { m_textures = textures; }

	/**
	 * @brief Habilita o deshabilita la capacidad del actor de proyectar sombras.
	 * @param v @c true para proyectar sombras, @c false para ignorar.
	 */
	void
		setCastShadow(bool v) { castShadow = v; }

	/**
	 * @brief Consulta si el actor tiene activada la proyección de sombras.
	 * @return @c true si proyecta sombras.
	 */
	bool
		canCastShadow() const { return castShadow; }

	/**
	 * @brief Renderiza el actor desde la perspectiva de la luz para generar el mapa de sombras.
	 * Utiliza shaders simplificados y estados de profundidad específicos.
	 * * @param deviceContext Contexto del dispositivo.
	 */
	void
		renderShadow(DeviceContext& deviceContext);

private:
	std::vector<MeshComponent> m_meshes;    ///< Geometría y sub-mallas del actor.
	std::vector<Texture> m_textures;        ///< Mapas de textura (Albedo, Normal, etc.).
	std::vector<Buffer> m_vertexBuffers;    ///< Almacenamiento de vértices en GPU.
	std::vector<Buffer> m_indexBuffers;     ///< Almacenamiento de índices en GPU.

	// Estados de Renderizado (Comentados según tu código original)
	//BlendState m_blendstate;              ///< Estado de mezcla de colores.
	//RasterizerState m_rasterizer;         ///< Configuración de rasterización (Culling, Wireframe).

	SamplerState m_sampler;                 ///< Configuración de filtrado de texturas.
	CBChangesEveryFrame m_model;            ///< Estructura de datos para la matriz de transformación.
	Buffer m_modelBuffer;                   ///< Buffer constante (GPU) para la transformación del modelo.

	// --- Recursos para Shadow Mapping ---
	ShaderProgram m_shaderShadow;           ///< Shader simplificado para el pase de profundidad de sombras.
	Buffer m_shaderBuffer;                  ///< Buffer para datos constantes del shader de sombras.
	//BlendState m_shadowBlendState;
	DepthStencilState m_shadowDepthStencilState; ///< Control de profundidad para el mapa de sombras.
	CBChangesEveryFrame m_cbShadow;         ///< Datos constantes específicos para el cálculo de sombras.

	XMFLOAT4 m_LightPos;                    ///< Posición de la fuente de luz para el cálculo de proyección.
	std::string m_name = "Actor";           ///< Identificador único o etiqueta del objeto.
	bool castShadow = true;                 ///< Flag de control para el sistema de iluminación.
};