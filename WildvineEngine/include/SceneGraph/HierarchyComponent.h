#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h"

// Forward declarations para reducir el acoplamiento
class DeviceContext;
class Entity;

/**
 * @class HierarchyComponent
 * @brief Gestiona las relaciones de parentesco entre entidades en la escena.
 * * Este componente permite construir una estructura de árbol (Scene Graph).
 * Facilita que las entidades tengan un padre y múltiples hijos, lo cual es esencial
 * para que las transformaciones (posición, rotación, escala) se propaguen correctamente
 * a través de la jerarquía.
 */
class
	HierarchyComponent : public Component {
public:
	/**
	 * @brief Constructor que inicializa el componente con el tipo HIERARCHY.
	 */
	HierarchyComponent() : Component(ComponentType::HIERARCHY) {}

	/**
	 * @brief Destructor por defecto.
	 */
	~HierarchyComponent() = default;

	/**
	 * @brief Inicialización del componente.
	 */
	void
		init() override {}

	/**
	 * @brief Actualización lógica.
	 * @note Actualmente no realiza operaciones por frame, pero puede extenderse.
	 */
	void
		update(float) override {}

	/**
	 * @brief Renderizado del componente.
	 * @note La jerarquía no tiene una representación visual directa.
	 */
	void
		render(DeviceContext& deviceContext) override {}

	/**
	 * @brief Limpia las relaciones de la entidad.
	 * Desvincula al padre y vacía la lista de hijos para evitar punteros colgados.
	 */
	void
		destroy() override {
		m_children.clear();
		m_parent = nullptr;
	}

	// --- API de SceneGraph ---

	/**
	 * @brief Asigna una entidad como padre de esta.
	 * @param parent Puntero a la entidad padre. @c nullptr si se desea convertir en raíz.
	 */
	void
		setParent(Entity* parent) {
		m_parent = parent;
	}

	/**
	 * @brief Comprueba si la entidad es el nodo raíz (no tiene padre).
	 * @return @c true si no tiene padre asignado.
	 */
	bool
		isRoot() const {
		return m_parent == nullptr;
	}

	/**
	 * @brief Comprueba si la entidad tiene entidades subordinadas.
	 * @return @c true si la lista de hijos no está vacía.
	 */
	bool
		hasChildren() const {
		return !m_children.empty();
	}

	/**
	 * @brief Agrega una entidad a la lista de hijos.
	 * Verifica que el hijo sea válido y que no haya sido agregado previamente.
	 * @param child Puntero a la entidad que será subordinada.
	 */
	void
		addChild(Entity* child) {
		if (!child) {
			return;
		}

		// Evita duplicados en la lista de hijos
		if (std::find(m_children.begin(), m_children.end(), child) != m_children.end()) {
			return;
		}
		m_children.push_back(child);
	}

	/**
	 * @brief Elimina una entidad específica de la lista de hijos.
	 * @param child Puntero al hijo que se desea desvincular.
	 */
	void
		removeChild(Entity* child) {
		if (!child) return;

		m_children.erase(
			std::remove(m_children.begin(), m_children.end(), child),
			m_children.end()
		);
	}

public:
	/**
	 * @brief Puntero a la entidad padre.
	 */
	Entity* m_parent = nullptr;

	/**
	 * @brief Lista de punteros a todas las entidades hijas.
	 */
	std::vector<Entity*> m_children;
};