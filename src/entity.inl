/** \brief Definitions for the templated functions declared in entity.hpp. */

/** \brief Compile-Time assert that T is derived from Component. */
template<class T> constexpr void check_T();

/** \brief Checks if the given T has a value set in the map. */
template<class T> constexpr bool exists_in_map(const Entity::ComponentMap& map);

/** \brief Maps from type T to a unique Key for the map. */
template<class T> const Entity::ComponentMap::key_type key_from_T();

template<class T>
bool
Entity::has_component() const
{
    check_T<T>();

    return exists_in_map<T>(components_) && components_.at(key_from_T<T>());
}

template<class T>
T*
Entity::add_component()
{
    check_T<T>();

    return set_component<T>(std::make_unique<T>(*this));
}

template<class T>
T*
Entity::set_component(std::unique_ptr<Component> component)
{
    check_T<T>();

    if (exists_in_map<T>(components_)) {
        components_.at(key_from_T<T>()) = std::move(component);
    } else {
        components_.insert(
            ComponentMap::value_type(key_from_T<T>(),
                                      std::move(component)));
    }

    return static_cast<T*>(components_.at(key_from_T<T>()).get());
}

template<class T>
T*
Entity::get_component() const
{
    check_T<T>();

    if (!exists_in_map<T>(components_)) {
        return nullptr;
    }

    return static_cast<T*>(components_.at(key_from_T<T>()).get());
}

template<class T> constexpr
void
check_T()
{
    static_assert(std::is_base_of<Component, T>(), "Require Component Subclass");
}

template<class T> constexpr
bool exists_in_map(const Entity::ComponentMap& map)
{
    return !!map.count(key_from_T<T>());
}

template<class T>
const Entity::ComponentMap::key_type
key_from_T()
{
    return std::type_index(typeid(T));
}
