#include "time.hpp"


cyan::WorldTimer::WorldTimer(double base_ms_per_tick)
        : _speed_mult(1.0),
          _base_ms_per_tick(base_ms_per_tick),
          _base_ticks_per_ms(1.0 / base_ms_per_tick),
          _elapsed_ticks(0.0)
{}

void cyan::WorldTimer::pause()
{
    _speed_mult = 0.0;
}

void cyan::WorldTimer::set_speed_multiplier(double speed_mult)
{
    _speed_mult = speed_mult;
}

double cyan::WorldTimer::ms_per_tick() const
{
    return _speed_mult * _base_ms_per_tick;
}

double cyan::WorldTimer::base_ms_per_tick() const
{
    return _base_ms_per_tick;
}

double cyan::WorldTimer::get_current_speed_multiplier() const
{
    return _speed_mult;
}

double cyan::WorldTimer::convert_ms_to_ticks(double ms) const
{
    return _speed_mult * ms * _base_ticks_per_ms;
}

double cyan::WorldTimer::get_elapsed_ticks() const
{
    return _elapsed_ticks;
}

void cyan::WorldTimer::record_elapsed_ticks(double ticks)
{
    _elapsed_ticks += ticks;
}

void cyan::WorldTimer::record_elapsed_ms(double ms)
{
    record_elapsed_ticks(convert_ms_to_ticks(ms));
}

cyan::WorldTimer cyan::WorldTimer::with_base_seconds_per_tick(double base_seconds_per_tick)
{
    auto base_ms_per_tick = 1000.0 * base_seconds_per_tick;
    return WorldTimer(base_ms_per_tick);
}
