from copy import copy
from pathlib import Path
from sys import path

# Path to the build directory including a file similar to 'ruckig.cpython-37m-x86_64-linux-gnu'.
build_path = Path(__file__).parent.absolute().parent / 'build'
path.insert(0, str(build_path))

from ruckig import InputParameter, OutputParameter, Result, Ruckig, ControlInterface, Synchronization


if __name__ == '__main__':
    # Create instances: the Ruckig OTG as well as input and output parameters
    otg = Ruckig(3, 0.01)  # DoFs, control cycle
    inp = InputParameter(3)
    out = OutputParameter(3)
    
    inp.current_position = [0.0, 0.0, 0.5]
    inp.current_velocity = [0.0, -2.2, -0.5]
    inp.current_acceleration = [0.0, 2.5, -0.5]

    inp.target_position = [-5.0, -2.0, -3.5]
    inp.target_velocity = [0.0, -0.5, -2.0]
    inp.target_acceleration = [0.0, 0.0, 0.5]

    inp.max_velocity = [3.0, 1.0, 3.0]
    inp.max_acceleration = [3.0, 2.0, 1.0]
    inp.max_jerk = [4.0, 3.0, 2.0]


    print('\t'.join(['t'] + [str(i) for i in range(otg.degrees_of_freedom)]))

    # Generate the trajectory within the control loop
    first_output = None
    on_stop_trajectory = False
    res = Result.Working
    while res == Result.Working:
        res = otg.update(inp, out)

        print('\t'.join([f'{out.time:0.3f}'] + [f'{p:0.3f}' for p in out.new_position]))

        # Activate stop trajectory after 1s
        if out.time >= 1.0 and not on_stop_trajectory:
            print('Stop immediately!')
            on_stop_trajectory = True

            # Synchronization is disabled so that each DoF stops as fast as possible independently
            inp.control_interface = ControlInterface.Velocity
            inp.synchronization = Synchronization.No
            inp.target_velocity = [0.0, 0.0, 0.0]
            inp.target_acceleration = [0.0, 0.0, 0.0]
            inp.max_jerk = [12.0, 10.0, 8.0]

        out.pass_to_input(inp)

        if not first_output:
            first_output = copy(out)

    print(f'Calculation duration: {first_output.calculation_duration:0.1f} [µs]')
    print(f'Trajectory duration: {first_output.trajectory.duration:0.4f} [s]')
