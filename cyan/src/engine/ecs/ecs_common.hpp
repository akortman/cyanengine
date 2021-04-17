/**
 * Common utilities and compile-time parameters for the ECS system.
 * This contains the details for the generation/index scheme: ID values within the ECS have a number of generation bits
 * followed by a number of index bits. The index bits specify the position of the entity/component within it's array.
 * The generation bits allow reuse of indicies without confusion about whether or not an ID can refer to a deleted
 * object.
 */

#pragma once

#include <cstdint>
#include <limits>

// Required to ensure some VC++ legacy macros don't make issues with numeric_limits::max()
#ifdef max
#undef max
#endif

namespace cyan {
    // Type aliases and utiltity values for # of bits and masks for use by the ECS generation/index split scheme.
    using EcsIdT = std::uint64_t;
    using EcsGenerationT = EcsIdT;
    using EcsIndexT = EcsIdT;
    constexpr std::uint64_t ENTITY_GENERATION_BITS = 32;
    constexpr std::uint64_t ECS_INDEX_BITS = std::numeric_limits<EcsIdT>::digits
                                                - ENTITY_GENERATION_BITS;
    constexpr std::uint64_t ECS_INDEX_MASK = ((~(EcsIdT) 0) >> ENTITY_GENERATION_BITS);
    constexpr std::uint64_t ECS_NULL_INDEX = std::numeric_limits<EcsIdT>::max() & ECS_INDEX_MASK;
    static_assert(ECS_INDEX_BITS > 0);
    static_assert(ENTITY_GENERATION_BITS > 0);
    static_assert(ECS_INDEX_BITS + ENTITY_GENERATION_BITS == std::numeric_limits<EcsIdT>::digits);
    static_assert(ECS_NULL_INDEX != std::numeric_limits<EcsIdT>::max());

    namespace ecs_impl {
        /** Extracts the generation number from an ECS id.
         * @param id The ECS id value
         * @return The generation value corresponding with the given ID
         */
        EcsGenerationT get_generation(EcsIdT id);

        /** Extracts the array index position from an ECS id.
         * @param id The ECS id value
         * @return The array index position corresponding with the given ID
         */
        EcsIndexT get_index(EcsIdT id);

        /**
         * Combine generation and index values into a resulting ID value.
         * @param generation The generation number
         * @param index The index value
         * @return A combined bitmanipulated ID value
         */
        EcsIdT make_ecs_id(EcsGenerationT generation, EcsIndexT index);
    }
}