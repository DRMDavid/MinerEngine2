#pragma once
#include "Prerequisites.h"

/**
 * @enum ResourceType
 * @brief Define las categorías de activos que el motor puede gestionar.
 */
enum class
	ResourceType {
	Unknown,    ///< Tipo no definido o error de identificación.
	Model3D,    ///< Mallas, objetos .obj, .fbx, etc.
	Texture,    ///< Mapas de bits (Albedo, Normal, Specular).
	Sound,      ///< Archivos de audio y efectos.
	Shader,     ///< Código de sombreadores (HLSL/GLSL).
	Material    ///< Definiciones de propiedades visuales y estados.
};

/**
 * @enum ResourceState
 * @brief Representa el ciclo de vida de un recurso en memoria.
 */
enum class
	ResourceState {
	Unloaded,   ///< El recurso existe en disco pero no en memoria.
	Loading,    ///< El recurso está en proceso de carga (útil para carga asíncrona).
	Loaded,     ///< El recurso está listo para ser utilizado por el motor.
	Failed      ///< Hubo un error crítico durante la carga.
};

/**
 * @class IResource
 * @brief Interfaz base para todos los recursos del motor.
 * * Define los métodos necesarios para la carga, inicialización y liberación de activos.
 * Cada recurso posee un ID único generado automáticamente para facilitar su búsqueda
 * y gestión en el sistema de archivos del motor.
 */
class IResource {
public:
	/**
	 * @brief Constructor que inicializa los metadatos básicos del recurso.
	 * @param name Nombre identificador del recurso.
	 */
	IResource(const std::string& name)
		: m_name(name)
		, m_filePath("")
		, m_type(ResourceType::Unknown)
		, m_state(ResourceState::Unloaded)
		, m_id(GenerateID())
	{
	}

	/**
	 * @brief Destructor virtual para asegurar la limpieza en clases derivadas.
	 */
	virtual ~IResource() = default;

	/**
	 * @brief Crea y configura el recurso en la GPU o API correspondiente.
	 * @return @c true si la inicialización fue exitosa.
	 */
	virtual bool init() = 0;

	/**
	 * @brief Carga los datos binarios o de texto desde el almacenamiento persistente.
	 * @param filename Ruta del archivo a cargar.
	 * @return @c true si el archivo se leyó correctamente.
	 */
	virtual bool load(const std::string& filename) = 0;

	/**
	 * @brief Libera la memoria (RAM y VRAM) ocupada por el recurso.
	 */
	virtual void unload() = 0;

	/**
	 * @brief Obtiene el peso del recurso en memoria para estadísticas y optimización.
	 * @return Tamaño del recurso en bytes.
	 */
	virtual size_t getSizeInBytes() const = 0;

	// --- Setters ---
	void SetPath(const std::string& path) { m_filePath = path; }
	void SetType(ResourceType t) { m_type = t; }
	void SetState(ResourceState s) { m_state = s; }

	// --- Getters ---
	const std::string& GetName() const { return m_name; }
	const std::string& GetPath() const { return m_filePath; }
	ResourceType GetType() const { return m_type; }
	ResourceState GetState() const { return m_state; }
	uint64_t GetID() const { return m_id; }

protected:
	std::string m_name;      ///< Nombre amigable del recurso.
	std::string m_filePath;  ///< Ruta absoluta o relativa en el disco.
	ResourceType m_type;     ///< Categoría del recurso.
	ResourceState m_state;   ///< Estado actual (Cargado, fallido, etc.).
	uint64_t m_id;           ///< Identificador único numérico.

private:
	/**
	 * @brief Genera un identificador incremental único para cada instancia de recurso.
	 * @return Un entero de 64 bits único.
	 */
	static uint64_t GenerateID()
	{
		static uint64_t nextID = 1;
		return nextID++;
	}
};