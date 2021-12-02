/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2020
 *
 * @file        pid.hpp
 * @brief       A generic, extensible PID loop.
 *
 * @author      Fuchs, Daniel <Daniel.Fuchs@toptica.com>
 * @author      Hager, Manfred <Manfred.Hager@toptica.com>
 * @author      Hempel, Felix <Felix.Hempel@toptica.com>
 * @author      Lopes, Emilio <Emilio.Lopes@toptica.com>
 * @author      Rehme, Paul <Paul.Rehme@toptica.com>
 * @author      Roggenbuck, Axel <Axel.Roggenbuck@toptica.com>
 * @author      Zhang, Xiaodong <Xiaodong.Zhang@toptica.com>
 *
 ******************************************************************************/
#pragma once

#include <boost/sml.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <tuple>
#include <type_traits>

namespace toptica::tsp::pid {

namespace state {
class start;
class idle;
class running;
class hold;
} // namespace state

namespace event {
class enable
{
};
class disable
{
};
class hold
{
};
class reset
{
};
} // namespace event

namespace anti_windup {

/*******************************************************************************
 * @class none
 *
 * @brief No anti-windup logic.
 ******************************************************************************/
template <typename T>
class none
{
    static_assert(std::is_floating_point<T>::value, "Only floating-point types are supported!");

  public:
    explicit constexpr none();

  protected:
    T    m_get_integrator(const T& integrator, const T& integrator_increment, const T& s);
    void m_reset();
};

/*******************************************************************************
 * @class conditional_integration
 *
 * @brief Anti-windup logic which uses conditional integration.
 ******************************************************************************/
template <typename T>
class conditional_integration : public none<T>
{
  protected:
    T m_get_integrator(const T& integrator, const T& integrator_increment, const T& s);
};

} // namespace anti_windup

/*******************************************************************************
 * @class pid
 *
 * @brief A generic, extensible PID loop.
 *
 * @details
 *     The integration part is calculated using trapezoidal discretization
 *     since this gives a flat phase response in the [bode diagram](@ref bode).
 *
 * @startuml "PID structure" width=5cm
 *     skinparam nodesep 80
 *
 *     interface "setpoint" as sp
 *     interface "process variable" as pv
 *     interface "control variable" as cv
 *     interface "error" as e
 *     interface "Configuration" as c
 *
 *     cloud "System (Plant)" as p
 *
 *     sp -> "+" e
 *     pv --> "-" e
 *     e -> [PID]
 *     c -up-> [PID]
 *     [PID] -> cv
 *     cv -up-> p
 *     p -left-> pv
 * @enduml
 *
 * @startuml{pid_state_diagram.png} "PID state diagram" width=5cm
 *     [*] --> idle: / m_hold = true;
 *     idle --> running: enable
 *     running --> idle: disable
 *     running --> running: reset / m_reset();
 *     running --> hold: hold
 *     hold --> running: enable
 *     hold --> idle: disable
 *
 *     idle: entry / m_reset();
 *     running: entry / m_hold = false;
 *     running: exit / m_hold = true;
 * @enduml
 *
 * @anchor bode
 * @image  html pid/compare_discretization.jpg "Bode diagram"
 *
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup = anti_windup::none>
class pid : private AntiWindup<T>
{
  public:
    explicit pid(const T& sampling_interval, const T& p = 0, const T& i = 0, const T& d = 0);

    T run(const T& error);

    void enable();
    void disable();
    void hold();
    void reset();

    const char* get_state() const;

    T                get_gain() const;
    void             set_gain(const T& value);
    T                get_p() const;
    void             set_p(const T& value);
    T                get_i() const;
    void             set_i(const T& value);
    T                get_d() const;
    void             set_d(const T& value);
    T                get_error() const;
    T                get_control_variable_minimum() const;
    void             set_control_variable_minimum(const T& value);
    T                get_control_variable_maximum() const;
    void             set_control_variable_maximum(const T& value);
    std::tuple<T, T> get_control_variable_limits() const;
    void             set_control_variable_limits(const T& minimum, const T& maximum);
    T                get_sampling_interval() const;
    void             set_sampling_interval(const T& value);
    bool             is_limited() const;

  private:
    T    m_sampling_interval;
    T    m_p;
    T    m_i;
    T    m_d;
    T    m_gain{1};
    T    m_error{};
    bool m_hold{true};
    struct control_variable_t
    {
        T minimum{std::numeric_limits<T>::lowest()};
        T maximum{std::numeric_limits<T>::max()};
        T value{};
    } m_control_variable{};
    struct delay_t
    {
        T i{};
        T integrator{};
        T error{};
        T s{};
    };
    struct coefficient_t
    {
        T p{};
        T i{};
        T d{};
    };
    std::array<delay_t, 2>       m_delays{};
    std::array<coefficient_t, 2> m_coefficients{};
    delay_t*                     m_delay{&m_delays[0]};
    coefficient_t*               m_coefficient{&m_coefficients[0]};

    void m_update_coefficient();
    void m_reset();

    struct transitions
    {
        // Clang complains about accessing members of an incomplete type when
        // calling a method of the `pid` class directly in the transition
        // callbacks.  This seems related to the way SML instantiates this
        // class.  Thus we use proxy methods to work around this issue.
        template <typename P>
        static void reset(P& p)
        {
            p.m_reset();
        }

        template <typename P>
        static void hold(P& p, bool value)
        {
            p.m_hold = value;
        }

        auto operator()() const noexcept
        {
            using namespace boost;
            return sml::make_transition_table(
                // clang-format off
                *sml::state<state::start>                                / [] {}                                    = sml::state<state::idle>
                ,sml::state<state::idle>    + sml::event<event::enable>  / [] {}                                    = sml::state<state::running>
                ,sml::state<state::running> + sml::event<event::disable> / [] {}                                    = sml::state<state::idle>
                ,sml::state<state::running> + sml::event<event::reset>   / [] (pid& self) { reset(self); }
                ,sml::state<state::running> + sml::event<event::hold>    / [] {}                                    = sml::state<state::hold>
                ,sml::state<state::hold>    + sml::event<event::enable>  / [] {}                                    = sml::state<state::running>
                ,sml::state<state::hold>    + sml::event<event::disable> / [] {}                                    = sml::state<state::idle>
                ,sml::state<state::idle>    + sml::on_entry<sml::_>      / [] (pid& self) { reset(self); }
                ,sml::state<state::running> + sml::on_entry<sml::_>      / [] (pid& self) { hold(self, false); }
                ,sml::state<state::running> + sml::on_exit<sml::_>       / [] (pid& self) { hold(self, true); }
                // clang-format on
            );
        }
    };

    boost::sml::sm<transitions> m_sm;
};

/*******************************************************************************
 * @param sampling_interval The sampling interval (time between two samples)
 *                          in [s]. t<sub>Sample</sub> = 1/f<sub>Sample</sub>.
 * @param p                 The K<sub>P</sub> coefficient.
 * @param i                 The K<sub>I</sub> coefficient.
 * @param d                 The K<sub>D</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
pid<T, AntiWindup>::pid(const T& sampling_interval, const T& p, const T& i, const T& d)
  : m_sampling_interval{sampling_interval}, m_p{p}, m_i{i}, m_d{d}, m_sm{(*this)}
{
    m_update_coefficient();
}

/*******************************************************************************
 * @brief                   Executes a single PID time step.
 *
 * @param error             The difference between a desired setpoint and a
 *                          measured process variable ().
 * @return                  The control variable.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::run(const T& error)
{
    m_error = error;

    if (!m_hold) {
        m_delay->integrator = AntiWindup<T>::m_get_integrator(
            m_delay->integrator, m_coefficient->i * error + m_delay->i * m_delay->error, m_delay->s);
        m_delay->i = m_coefficient->i;

        auto control_variable{
            m_gain * (m_coefficient->p * error + m_delay->integrator + m_coefficient->d * (error - m_delay->error))};
        m_delay->error = error;

        // limit the control variable
        m_control_variable.value = std::clamp(control_variable, m_control_variable.minimum, m_control_variable.maximum);

        // calculate saturation value
        m_delay->s = m_control_variable.value - control_variable;
    }

    return m_control_variable.value;
}

/*******************************************************************************
 * @brief                   Enables the PID loop.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::enable()
{
    m_sm.process_event(event::enable{});
}
/*******************************************************************************
 * @brief                   Disables the PID loop.
 *
 * @details                 All internal parameters are reset to zero.
 *                          A subsequent call of pid::run will not change the
 *                          internal parameters and the return value will be
 *                          zero.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::disable()
{
    m_sm.process_event(event::disable{});
}
/*******************************************************************************
 * @brief                   Holds the PID loop.
 *
 * @details                 All internal parameters are frozen.
 *                          A subsequent call of pid::run will not change the
 *                          internal parameters and the return value will be
 *                          the last calculated one.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::hold()
{
    m_sm.process_event(event::hold{});
}
/*******************************************************************************
 * @brief                   Resets the PID loop.
 *
 * @details                 All internal parameters are reset to zero.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::reset()
{
    m_sm.process_event(event::reset{});
}

/*******************************************************************************
 * @brief                   Returns the current state of the state machine.
 *
 * @return                  The current state.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
const char* pid<T, AntiWindup>::get_state() const
{
    const char* name{};
    m_sm.visit_current_states([&](auto state) { name = state.c_str(); });
    const auto* ptr{std::strrchr(name, ':')};
    return (ptr != nullptr) ? &ptr[1] : name; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}

/*******************************************************************************
 * @brief                   Returns the overall gain of the PID loop.
 *
 * @return                  The overall gain.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_gain() const
{
    return m_gain;
}
/*******************************************************************************
 * @brief                   Sets the overall gain of the PID loop.
 *
 * @param value             The overall gain.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_gain(const T& value)
{
    m_gain = value;
}
/*******************************************************************************
 * @brief                   Returns the K<sub>P</sub> coefficient of the PID
 *                          loop.
 *
 * @return                  The K<sub>P</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_p() const
{
    return m_p;
}
/*******************************************************************************
 * @brief                   Sets the K<sub>P</sub> coefficient of the PID loop.
 *
 * @param value             The K<sub>P</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_p(const T& value)
{
    if (m_p != value) {
        m_p = value;
        m_update_coefficient();
    }
}
/*******************************************************************************
 * @brief                   Returns the K<sub>I</sub> coefficient of the PID
 *                          loop.
 *
 * @return                  The K<sub>I</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_i() const
{
    return m_i;
}
/*******************************************************************************
 * @brief                   Sets the K<sub>I</sub> coefficient of the PID loop.
 *
 * @param value             The K<sub>I</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_i(const T& value)
{
    if (m_i != value) {
        m_i = value;
        m_update_coefficient();
    }
}
/*******************************************************************************
 * @brief                   Returns the K<sub>D</sub> coefficient of the PID
 *                          loop.
 *
 * @return                  The K<sub>D</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_d() const
{
    return m_d;
}
/*******************************************************************************
 * @brief                   Sets the K<sub>D</sub> coefficient of the PID loop.
 *
 * @param value             The K<sub>D</sub> coefficient.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_d(const T& value)
{
    if (m_d != value) {
        m_d = value;
        m_update_coefficient();
    }
}
/*******************************************************************************
 * @brief                   Returns the last error which was fed to the PID
 *                          loop.
 *
 * @return                  The last error.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_error() const
{
    return m_error;
}
/*******************************************************************************
 * @brief                   Returns the control variable minimum limitation.
 *
 * @return                  The control variable minimum limitation.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_control_variable_minimum() const
{
    return m_control_variable.minimum;
}
/*******************************************************************************
 * @brief                   Sets the control variable minimum limitation.
 *
 * @param value             The control variable minimum limitation.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_control_variable_minimum(const T& value)
{
    m_control_variable.minimum = value;
}
/*******************************************************************************
 * @brief                   Returns the control variable maximum limitation.
 *
 * @return                  The control variable maximum limitation.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_control_variable_maximum() const
{
    return m_control_variable.maximum;
}
/*******************************************************************************
 * @brief                   Sets the control variable maximum limitation.
 *
 * @param value             The control variable maximum limitation.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_control_variable_maximum(const T& value)
{
    m_control_variable.maximum = value;
}
/*******************************************************************************
 * @brief                   Returns the control variable limitations.
 *
 * @return                  Tuple of the control variable minimum and maximum
 *                          limitation.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
std::tuple<T, T> pid<T, AntiWindup>::get_control_variable_limits() const
{
    return std::make_tuple(m_control_variable.minimum, m_control_variable.maximum);
}
/*******************************************************************************
 * @brief                   Sets the control variable limitations.
 *
 * @param minimum           The control variable minimum limitation.
 * @param maximum           The control variable maximum limitation.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_control_variable_limits(const T& minimum, const T& maximum)
{
    m_control_variable.minimum = minimum;
    m_control_variable.maximum = maximum;
}
/*******************************************************************************
 * @brief                   Returns the sampling interval.
 *
 * @return                  The sampling interval.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
T pid<T, AntiWindup>::get_sampling_interval() const
{
    return m_sampling_interval;
}
/*******************************************************************************
 * @brief                   Sets the sampling interval.
 *
 * @param value             The sampling interval.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::set_sampling_interval(const T& value)
{
    if (m_sampling_interval != value) {
        m_sampling_interval = value;
        m_update_coefficient();
    }
}
/*******************************************************************************
 * @brief                   Returns if the control variable is limited.
 *
 * @return                  True if the control variable is limited.
 ******************************************************************************/
template <typename T, template <typename> typename AntiWindup>
bool pid<T, AntiWindup>::is_limited() const
{
    return m_delay->s != 0;
}

template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::m_update_coefficient()
{
    auto coefficient{&m_coefficients.at((m_coefficient == &m_coefficients[0]) ? 1 : 0)};

    coefficient->p = m_p;
    coefficient->i = m_i * m_sampling_interval / 2;
    coefficient->d = m_d / m_sampling_interval;

    m_coefficient = coefficient;
}

template <typename T, template <typename> typename AntiWindup>
void pid<T, AntiWindup>::m_reset()
{
    auto delay{&m_delays.at((m_delay == &m_delays[0]) ? 1 : 0)};

    delay->integrator = 0;
    delay->error      = 0;
    delay->s          = 0;

    m_delay = delay;
}

template <typename T>
constexpr anti_windup::none<T>::none() = default;

template <typename T>
T anti_windup::none<T>::m_get_integrator(const T& integrator, const T& integrator_increment, const T& s)
{
    (void)s;
    return integrator + integrator_increment;
}

template <typename T>
void anti_windup::none<T>::m_reset()
{
}

template <typename T>
T anti_windup::conditional_integration<T>::m_get_integrator(
    const T& integrator, const T& integrator_increment, const T& s)
{
    auto value{integrator};

    // anti-windup for integrator part: conditional integrator
    if ((s == 0) || ((integrator * integrator_increment) <= 0)) {
        value += integrator_increment;
    }

    return value;
}

} // namespace toptica::tsp::pid
