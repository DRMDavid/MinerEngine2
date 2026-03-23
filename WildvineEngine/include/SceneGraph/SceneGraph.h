#pragma once
#include "Prerequisites.h"

// Forward declarations
class Entity;
class DeviceContext;

/**
 * @class SceneGraph
 * @brief Orquestador de la jerarquía y transformación de todas las entidades en la escena.
 * * El SceneGraph (Grafo de Escena) gestiona cómo las entidades se relacionan entre sí.
 * Su función principal es el recorrido recursivo para actualizar las matrices de
 * transformación del mundo, asegurando que si un padre se mueve, sus hijos lo sigan
 * manteniendo su posición relativa.
 */
class
	SceneGraph {
public:
	/**
	 * @brief Constructor por defecto.
	 */
	SceneGraph() = default;

	/**
	 * @brief Destructor por defecto.
	 */
	~SceneGraph() = default;

	/**
	 * @brief Inicializa los sistemas internos del grafo de escena.
	 */
	void
		init();

	/**
	 * @brief Registra una nueva entidad en el sistema del grafo.
	 * @param e Puntero a la entidad que se desea añadir al control del grafo.
	 */
	void
		addEntity(Entity* e);

	/**
	 * @brief Elimina una entidad del grafo y limpia sus referencias de parentesco.
	 * @param e Puntero a la entidad que se desea remover.
	 */
	void
		removeEntity(Entity* e);

	/**
	 * @brief Verifica si una entidad es ancestro (padre, abuelo, etc.) de otra.
	 * Útil para evitar ciclos infinitos (que un padre intente ser hijo de su propio hijo).
	 * @param possibleAncestor La entidad que podría ser el ancestro.
	 * @param node La entidad hija a comprobar.
	 * @return @c true si existe una relación de ancestro.
	 */
	bool
		isAncestor(Entity* possibleAncestor, Entity* node) const;

	/**
	 * @brief Establece una relación padre-hijo entre dos entidades.
	 * @param child La entidad que será subordinada.
	 * @param parent La entidad que actuará como padre.
	 * @return @c true si la operación de adjuntar fue exitosa.
	 */
	bool
		attach(Entity* child, Entity* parent);

	/**
	 * @brief Separa una entidad de su padre actual, convirtiéndola en un nodo raíz.
	 * @param child La entidad que se desea independizar.
	 * @return @c true si se logró desvincular con éxito.
	 */
	bool
		detach(Entity* child);

	/**
	 * @brief Actualiza todas las entidades del grafo.
	 * Inicia el proceso de actualización de lógica y el cálculo de matrices recursivas.
	 * @param deltaTime Tiempo transcurrido desde el último frame.
	 * @param deviceContext Contexto para operaciones que requieran acceso a la GPU.
	 */
	void
		update(float deltaTime, DeviceContext& deviceContext);

	/**
	 * @brief Envía a renderizar todas las entidades registradas en el grafo.
	 * @param deviceContext Contexto del dispositivo para comandos de dibujo.
	 */
	void
		render(DeviceContext& deviceContext);

	/**
	 * @brief Limpia por completo el grafo y libera las referencias a las entidades.
	 */
	void
		destroy();

private:
	/**
	 * @brief Método principal de recursión para calcular transformaciones globales.
	 * Multiplica la matriz local del nodo por la matriz global del padre.
	 * @param node Entidad actual en el recorrido.
	 * @param parentWorld Matriz de transformación acumulada del padre.
	 */
	void
		updateWorldRecursive(Entity* node, const XMMATRIX& parentWorld);

	/**
	 * @brief Comprueba si una entidad no tiene padre dentro de este grafo.
	 * @param e Puntero a la entidad.
	 * @return @c true si es un nodo raíz.
	 */
	bool
		isRoot(Entity* e) const;

	/**
	 * @brief Verifica si una entidad ya está registrada en la lista del grafo.
	 * @param e Puntero a la entidad.
	 * @return @c true si la entidad ya existe en @c m_entities.
	 */
	bool
		isRegistered(Entity* e) const;

private:
	// Nota: Considerar el uso de TSharedPointer para una gestión de memoria más robusta.
	// std::vector<EU::TSharedPointer<Entity>> m_entities;

public:
	/**
	 * @brief Contenedor de todas las entidades que pertenecen a la escena actual.
	 */
	std::vector<Entity*> m_entities;
};