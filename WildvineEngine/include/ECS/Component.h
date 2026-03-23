#pragma once
#include "Prerequisites.h"

// Forward declaration para evitar dependencias innecesarias en el header
class DeviceContext;

/**
 * @class Component
 * @brief Clase base abstracta para todos los componentes del motor.
 *
 * La clase Component define la interfaz fundamental que deben seguir todos los sistemas
 * adjuntos a una entidad (como mallas, luces o scripts). Al ser abstracta, obliga a las
 * clases derivadas a implementar su propia lógica de inicialización, actualización y renderizado.
 */
class
	Component {
public:
	/**
	 * @brief Constructor por defecto.
	 */
	Component() = default;

	/**
	 * @brief Constructor que asigna un tipo específico al componente.
	 * @param type Identificador del tipo de componente (proveniente de ComponentType).
	 */
	Component(const ComponentType type) : m_type(type) {}

	/**
	 * @brief Destructor virtual.
	 * Crucial para asegurar que los recursos de las clases derivadas se liberen correctamente.
	 */
	virtual
		~Component() = default;

	/**
	 * @brief Inicializa los recursos internos del componente.
	 * Debe implementarse para configurar estados iniciales o cargar datos necesarios.
	 */
	virtual void
		init() = 0;

	/**
	 * @brief Actualiza la lógica interna del componente en cada frame.
	 * @param deltaTime Tiempo transcurrido (en segundos) desde la última actualización.
	 */
	virtual void
		update(float deltaTime) = 0;

	/**
	 * @brief Ejecuta las operaciones de dibujado o envío de comandos a la GPU.
	 * @param deviceContext Referencia al contexto del dispositivo para el manejo de la API gráfica.
	 */
	virtual void
		render(DeviceContext& deviceContext) = 0;

	/**
	 * @brief Libera o detiene de forma segura el componente antes de su eliminación.
	 */
	virtual void
		destroy() = 0;

	/**
	 * @brief Obtiene el tipo de componente para identificación en tiempo de ejecución.
	 * @return El valor de la enumeración @c ComponentType asociado a esta instancia.
	 */
	ComponentType
		getType() const { return m_type; }

protected:
	/**
	 * @brief Almacena la categoría o tipo del componente.
	 * Accesible para clases hijas para definir su identidad.
	 */
	ComponentType m_type;
};