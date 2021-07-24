#pragma once


namespace cyan {
    struct WorldTimer {
        WorldTimer() = delete;

        /// Create a WorldTimer with a set base number of milliseconds per tick (then modified by speed mult).
        explicit WorldTimer(double base_ms_per_tick);

        /// Pause the game time.
        void pause();

        /// Set the game speed multiplier.
        void set_speed_multiplier(double speed_mult);

        /// Get the current ms/day value.
        [[nodiscard]] double ms_per_tick() const;

        /// Get the base ms/tick value, unaffected by the speed mult.
        [[nodiscard]] double base_ms_per_tick() const;

        /// Get the current value of the speed multiplier.
        [[nodiscard]] double get_current_speed_multiplier() const;

        /// Convert a number of milliseconds to days at the current ms_per_day rate.
        [[nodiscard]] double convert_ms_to_ticks(double ms) const;

        /// Get the number of days that have elapsed since clock start.
        [[nodiscard]] double get_elapsed_ticks() const;

        /// Record a number of days that have elapsed (this is added onto the clock's internal elapsed day count).
        void record_elapsed_ticks(double ticks);

        /// Record the elapsed milliseonds as days, converted at the current ms_per_day rate (this is added onto the
        /// clock's internal elapsed day count).
        void record_elapsed_ms(double ms);

        /// Construct and return a WorldTimer with a specified base number of seconds per day.
        static WorldTimer with_base_seconds_per_tick(double base_seconds_per_tick);

    private:
        double _speed_mult;
        const double _base_ms_per_tick;
        // To avoid a floating point division op every single tick, we save days/ms as well as ms/day.
        const double _base_ticks_per_ms;
        double _elapsed_ticks;
    };
}