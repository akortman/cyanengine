ECS Architecture
---
The ECS is a collection of:
    - Entities: These are simple IDs that are either active or not.
    - Core components: These are components in native (C++) code. Typically for engine internals.
    - Custom components: These are components with a more freeform structure meant for using with chai code. Typically
      for game-related data.