#pragma once

#include "cyan/src/engine/ecs/ecs_common.hpp"
#include "cyan/src/logging/assert.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

namespace cyan::ecs_impl {
    /** ObjectRegistry
     * Provides a reusable array of objects with unique identifiers.
     * Identifiers combine an index within an internal array and a generation value to prevent incidences of deleting
     * an object and having it's index reused causing errors.
     * Important note: Object registry functions return Entries, which are simple pointer + id combinations.
     * You should save the ID of an object, NOT the entry - the entry pointer may become invalidated. When you want to
     * use an object, the entry should be obtained via ID lookup (see get() function).
     * @tparam T The type of object to be stored.
     */
    template <typename T>
    struct ObjectRegistry {
        /**
         * ObjectRegistry<T>::Entry objects are used to refer to values within the array.
         * These act as a pointer to the value, as well as holding whether the value is active.
         */
        struct Entry {
            EcsIndexT id = ECS_NULL_INDEX;
            T* value;

            /// operator* overloads to allow this object to be used as if it's a pointer.
            T& operator*() { return *value; }
            const T& operator*() const { return *value; }

            /// Boolean conversion to check if the entry is valid.
            explicit operator bool() { return value != nullptr; }
        };

        /**
         * Add a copy of an object to the array and return a reference entry.
         * TODO: We probably want an rvalue-reference argument option.
         * @param object The object to make a copy of.
         * @return An entry which can be used to retrieve the value.
         */
        Entry add(const T& object) {
            CYAN_ASSERT(entries.size() == object_array.size());

            EcsIndexT target_index = ECS_NULL_INDEX;

            if (!empty_indices.empty()) {
                // If we have empty indices, use those first.
                target_index = empty_indices.front();
                empty_indices.pop();
                CYAN_ASSERT(!entries[target_index]);
                entries[target_index].id = make_ecs_id(
                        get_generation(entries[target_index].id) + 1,
                        target_index);
                entries[target_index].value = &object_array[target_index];
                object_array[target_index] = object;
            } else {
                // Otherwise, we allocate a new slot for the component.
                target_index = object_array.size();
                object_array.push_back(object);
                entries.push_back({target_index, &object_array[target_index]});
            }

            return entries[target_index];
        }

        /**
         * Create an object in-place and return a reference entry.
         * @tparam Args The types to construct the object from.
         * @param args The values to construct the object from.
         * @return An entry which can be used to retrieve the value.
         */
        template <typename ...Args>
        Entry emplace(Args... args) {
            CYAN_ASSERT(entries.size() == object_array.size());

            EcsIndexT target_index;

            if (!empty_indices.empty()) {
                // If we have empty indices, use those first.
                target_index = empty_indices.front();
                empty_indices.pop();
                CYAN_ASSERT(!entries[target_index]);
                entries[target_index].id = make_ecs_id(
                        get_generation(component_entries[target_index].id) + 1,
                        target_index);
                entries[target_index].value = &object_array[target_index];
                object_array[target_index] = T(args...);
            } else {
                // Otherwise, we allocate a new slot for the component.
                target_index = components.size();
                object_array.emplace_back(std::forward(args...));
                entries.emplace_back(e, target_index, &object_array[target_index]);
            }

            return entries[target_index];
        }

        /**
         * Look up an entry from an ID.
         * @param id The id of the object.
         * @return
         */
        Entry get(EcsIndexT id) {
            CYAN_ASSERT(entries.size() == object_array.size());

            auto index = get_index(id);

            // Make sure the provided index is exists within the array.
            if (index >= entries.size()) {
                return make_null_entry();
            }

            // If the ID doesn't match (i.e. the generation of the object) or the value pointer is null, we don't have
            // the requested object.
            Entry& entry = entries[index];
            if (entry.value == nullptr || entry.id != id) {
                return make_null_entry();
            }

            // add() and emplace() calls can invalidate our pointers within entries.
            // This is fine - we don't use the pointers internally, they're for use by the caller, but it means we have
            // to ensure that an entry has an up-to-date pointer on each get() call.
            entry.value = &object_array[index];

            return entry;
        }

        /**
         * Remove an object from the array.
         * @param id The id of the object to remove.
         */
        void remove(EcsIndexT id) {
            CYAN_ASSERT(entries.size() == object_array.size());

            auto index = get_index(id);

            // Make sure the provided index is exists within the array.
            if (index >= entries.size()) return;

            // If the ID doesn't match (i.e. the generation of the object) or the value pointer is null, we don't have
            // the requested object, so no deletion should occur.
            Entry& entry = entries[index];
            if (entry.value == nullptr || entry.id != id) {
                return;
            }

            // Ensure this object index is stored as free-to-use.
            empty_indices.push(index);

            // Object removal is as simple as setting the entry pointer to null - this will allow it to be overwritten
            // when needed (as long as no-one has saved a pointer to the object instead of using an ID, which is not
            // what should be happening).
            entry.value = nullptr;
        }

        /**
         * Get the number of currently active elements in the registry.
         */
        [[nodiscard]]
        std::size_t size() const {
            CYAN_ASSERT(entries.size() == object_array.size());
            return entries.size() - empty_indices.size();
        }

    private:
        std::vector<Entry> entries{};
        std::vector<T> object_array;
        std::queue<EcsIndexT> empty_indices;

        /**
         * Internal function to make a null entry.
         */
        Entry make_null_entry() const {
            return Entry{ECS_NULL_INDEX, nullptr};
        }
    };
}