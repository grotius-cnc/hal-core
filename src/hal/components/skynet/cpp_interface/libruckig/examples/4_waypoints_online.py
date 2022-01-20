from copy import copy
from pathlib import Path
from sys import path

# Path to the build directory including a file similar to 'ruckig.cpython-37m-x86_64-linux-gnu'.
build_path = Path(__file__).parent.absolute().parent / 'build'
path.insert(0, str(build_path))

from ruckig import InputParameter, OutputParameter, Result, Ruckig


if __name__ == '__main__':
    # Create instances: the Ruckig OTG as well as input and output parameters
    otg = Ruckig(3, 0.01, 10)  # DoFs, control cycle rate, maximum number of intermediate waypoints for memory allocation
    inp = InputParameter(3)  # DoFs
    out = OutputParameter(3, 10)  # DoFs, maximum number of intermediate waypoints for memory allocation

    inp.current_position = [0.2, 0, -0.3]
    inp.current_velocity = [0, 0.2, 0]
    inp.current_acceleration = [0, 0.6, 0]

    inp.intermediate_positions = [
        [1.4, -1.6, 1.0],
        [-0.6, -0.5, 0.4],
        [-0.4, -0.35, 0.0],
        [0.8, 1.8, -0.1]
    ]

    inp.target_position = [0.5, 1, 0]
    inp.target_velocity = [0.2, 0, 0.3]
    inp.target_acceleration = [0, 0.1, -0.1]

    inp.max_velocity = [1, 2, 1]
    inp.max_acceleration = [3, 2, 2]
    inp.max_jerk = [6, 10, 20]

    inp.interrupt_calculation_duration = 500  # [µs]


    print('\t'.join(['t'] + [str(i) for i in range(otg.degrees_of_freedom)]))

    # Generate the trajectory within the control loop
    first_output = None
    res = Result.Working
    while res == Result.Working:
        res = otg.update(inp, out)

        print('\t'.join([f'{out.time:0.3f}'] + [f'{p:0.3f}' for p in out.new_position]))

        out.pass_to_input(inp)

        if not first_output:
            first_output = copy(out)

    print(f'Calculation duration: {first_output.calculation_duration:0.1f} [µs]')
    print(f'Trajectory duration: {first_output.trajectory.duration:0.4f} [s]')
