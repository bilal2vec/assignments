import random
import numpy as np
import sys
import math
from copy import deepcopy
import matplotlib.pyplot as plt


class Sender:
    """
    A Q-learning agent that sends messages to a Receiver

    """

    def __init__(self, num_sym: int, grid_rows: int, grid_cols: int,
                 alpha_i: float, alpha_f: float, num_ep: int, epsilon: float,
                 discount: float):
        """
        Initializes this agent with a state, set of possible actions, and a means of storing Q-values

        :param num_sym: The number of arbitrary symbols available for sending
        :type num_sym: int
        :param grid_rows: The number of rows in the grid
        :type grid_rows: int
        :param grid_cols: The number of columns in the grid
        :type grid_cols: int
        :param alpha_i: The initial learning rate
        :type alpha: float
        :param alpha_f: The final learning rate
        :type alpha: float
        :param num_ep: The total number of episodes
        :type num_ep: int
        :param epsilon: The epsilon in epsilon-greedy exploration
        :type epsilon: float
        :param discount: The discount factor
        :type discount: float
        """
        self.actions = range(num_sym)
        self.alpha = alpha_i
        self.alpha_i = alpha_i
        self.alpha_f = alpha_f
        self.num_ep = num_ep
        self.epsilon = epsilon
        self.discount = discount
        self.q_vals = np.zeros((grid_rows * grid_cols, num_sym))
        self.grid_rows = grid_rows
        self.grid_cols = grid_cols

    def state2idx(self, state):
        return state[1] * self.grid_cols + state[0]

    def select_action(self, state):
        """
        This function is called every time the agent must act. It produces the action that the agent will take
        based on its current state

        :param state: the state the agent is acting from, in the form (x,y), which are the coordinates of the prize
        :type state: (int, int)
        :return: The symbol to be transmitted (must be an int < N)
        :rtype: int
        """
        state_idx = self.state2idx(state)
        if random.random() < self.epsilon:
            return random.choice(self.actions)
        else:
            if np.all(self.q_vals[state_idx, :] == self.q_vals[state_idx, 0]):
                return random.choice(self.actions)
            return np.argmax(self.q_vals[state_idx, :])

    def update_q(self, old_state, action, reward):
        """
        This function is called after an action is resolved so that the agent can update its Q-values

        :param old_state: the state the agent was in when it acted, in the form (x,y), which are the coordinates
                          of the prize
        :type old_state: (int, int)
        :param action: the action that was taken
        :type action: int
        :param reward: the reward that was received
        :type reward: float
        """
        old_state_idx = self.state2idx(old_state)
        old_q_val = self.q_vals[old_state_idx, action]
        self.q_vals[old_state_idx,
                    action] = old_q_val + self.alpha * (reward - old_q_val)


class Receiver:
    """
    A Q-learning agent that receives a message from a Sender and then navigates a grid

    """

    def __init__(self, num_sym: int, grid_rows: int, grid_cols: int,
                 alpha_i: float, alpha_f: float, num_ep: int, epsilon: float,
                 discount: float):
        """
        Initializes this agent with a state, set of possible actions, and a means of storing Q-values

        :param num_sym: The number of arbitrary symbols available for sending
        :type num_sym: int
        :param grid_rows: The number of rows in the grid
        :type grid_rows: int
        :param grid_cols: The number of columns in the grid
        :type grid_cols: int
        :param alpha_i: The initial learning rate
        :type alpha: float
        :param alpha_f: The final learning rate
        :type alpha: float
        :param num_ep: The total number of episodes
        :type num_ep: int
        :param epsilon: The epsilon in epsilon-greedy exploration
        :type epsilon: float
        :param discount: The discount factor
        :type discount: float
        """
        self.actions = [0, 1, 2,
                        3]  # Note: these correspond to [up, down, left, right]
        self.alpha = alpha_i
        self.alpha_i = alpha_i
        self.alpha_f = alpha_f
        self.num_ep = num_ep
        self.epsilon = epsilon
        self.discount = discount
        self.q_vals = np.zeros(
            (num_sym * grid_rows * grid_cols, len(self.actions)))
        self.grid_rows = grid_rows
        self.grid_cols = grid_cols
        self.num_sym = num_sym

    def state2idx(self, state):
        msg, x, y = state
        return msg * (self.grid_rows * self.grid_cols) + y * self.grid_cols + x

    def select_action(self, state):
        """
        This function is called every time the agent must act. It produces the action that the agent will take
        based on its current state
        :param state: the state the agent is acting from, in the form (m,x,y), where m is the message received
                      and (x,y) are the board coordinates
        :type state: (int, int, int)
        :return: The direction to move, where 0 is up, 1 is down, 2 is left, and 3 is right
        :rtype: int
        """
        state_idx = self.state2idx(state)
        if random.random() < self.epsilon:
            return random.choice(self.actions)
        else:
            if np.all(self.q_vals[state_idx, :] == self.q_vals[state_idx, 0]):
                return random.choice(self.actions)
            return np.argmax(self.q_vals[state_idx, :])

    def update_q(self, old_state, new_state, action, reward):
        """
        This function is called after an action is resolved so that the agent can update its Q-values

        :param old_state: the state the agent was in when it acted in the form (m,x,y), where m is the message received
                          and (x,y) are the board coordinates
        :type old_state: (int, int, int)
        :param new_state: the state the agent entered after it acted
        :type new_state: (int, int, int)
        :param action: the action that was taken
        :type action: int
        :param reward: the reward that was received
        :type reward: float
        """
        old_state_idx = self.state2idx(old_state)
        new_state_idx = self.state2idx(new_state)

        old_q_val = self.q_vals[old_state_idx, action]
        new_max_q = np.max(self.q_vals[new_state_idx, :])

        self.q_vals[old_state_idx, action] = old_q_val + self.alpha * (
            reward + self.discount * new_max_q - old_q_val)


def get_grid(grid_name: str):
    """
    This function produces one of the three grids defined in the assignment as a nested list

    :param grid_name: the name of the grid. Should be one of 'fourroom', 'maze', or 'empty'
    :type grid_name: str
    :return: The corresponding grid, where True indicates a wall and False a space
    :rtype: list[list[bool]]
    """
    grid = [[False for i in range(5)] for j in range(5)
           ]  # default case is 'empty'
    if grid_name == 'fourroom':
        grid[0][2] = True
        grid[2][0] = True
        grid[2][1] = True
        grid[2][3] = True
        grid[2][4] = True
        grid[4][2] = True
    elif grid_name == 'maze':
        grid[1][1] = True
        grid[1][2] = True
        grid[1][3] = True
        grid[2][3] = True
        grid[3][1] = True
        grid[4][1] = True
        grid[4][2] = True
        grid[4][3] = True
        grid[4][4] = True
    return grid


def legal_move(posn_x: int, posn_y: int, move_id: int, grid: list[list[bool]]):
    """
    Produces the new position after a move starting from (posn_x,posn_y) if it is legal on the given grid (i.e. not
    out of bounds or into a wall)

    :param posn_x: The x position (column) from which the move originates
    :type posn_x: int
    :param posn_y: The y position (row) from which the move originates
    :type posn_y: int
    :param move_id: The direction to move, where 0 is up, 1 is down, 2 is left, and 3 is right
    :type move_id: int
    :param grid: The grid on which to move, where False indicates a space and True a wall
    :type grid: list[list[bool]]
    :return: The new (x,y) position if the move was legal, or the old position if it was not
    :rtype: (int, int)
    """
    moves = [[0, -1], [0, 1], [-1, 0], [1, 0]]
    new_x = posn_x + moves[move_id][0]
    new_y = posn_y + moves[move_id][1]
    result = (new_x, new_y)
    if new_x < 0 or new_y < 0 or new_x >= len(grid[0]) or new_y >= len(grid):
        result = (posn_x, posn_y)
    else:
        if grid[new_y][new_x]:
            result = (posn_x, posn_y)
    return result


def run_episodes(sender: Sender, receiver: Receiver, grid: list[list[bool]],
                 num_ep: int, delta: float):
    """
    Runs the reinforcement learning scenario for the specified number of episodes

    :param sender: The Sender agent
    :type sender: Sender
    :param receiver: The Receiver agent
    :type receiver: Receiver
    :param grid: The grid on which to move, where False indicates a space and True a wall
    :type grid: list[list[bool]]
    :param num_ep: The number of episodes
    :type num_ep: int
    :param delta: The chance of termination after every step of the receiver
    :type delta: float [0,1]
    :return: A list of the reward received by each agent at the end of every episode
    :rtype: list[float]
    """
    reward_vals = []

    # Episode loop
    for ep in range(num_ep):
        # Set receiver starting position
        receiver_x = 2
        receiver_y = 2

        # Choose prize position
        prize_x = np.random.randint(len(grid[0]))
        prize_y = np.random.randint(len(grid))
        while grid[prize_y][prize_x] or (prize_x == receiver_x and
                                         prize_y == receiver_y):
            prize_x = np.random.randint(len(grid[0]))
            prize_y = np.random.randint(len(grid))

        # Initialize new episode

        sender_state = (prize_x, prize_y)
        message = sender.select_action(sender_state)
        eps_reward = 0.0

        # Receiver loop
        terminate = False

        receiver_state = (message, receiver_x, receiver_y)

        while not terminate:
            receiver_action = receiver.select_action(receiver_state)
            new_receiver_x, new_receiver_y = legal_move(receiver_x, receiver_y,
                                                        receiver_action, grid)
            new_receiver_state = (message, new_receiver_x, new_receiver_y)

            reward = 0.0
            if new_receiver_x == prize_x and new_receiver_y == prize_y:
                reward = 1.0
                eps_reward = 1.0
                terminate = True

            if not terminate and random.random() < delta:
                terminate = True

            receiver.update_q(receiver_state, new_receiver_state,
                              receiver_action, reward)

            receiver_x, receiver_y = new_receiver_x, new_receiver_y
            receiver_state = new_receiver_state

        sender.update_q(sender_state, message, eps_reward)

        alpha_decay = (sender.alpha_i - sender.alpha_f) / sender.num_ep
        sender.alpha = max(sender.alpha_f, sender.alpha - alpha_decay)
        receiver.alpha = max(receiver.alpha_f, receiver.alpha - alpha_decay)

        reward_vals.append(eps_reward)

    return reward_vals


def plot_results(nep_values,
                 results,
                 title,
                 xlabel='log(Nep)',
                 ylabel='Average Discounted Reward'):
    """
    Generates plots for the experimental results.

    :param nep_values: List of Nep values used.
    :param results: Dictionary mapping labels (e.g., epsilon values) to tuples of (means, std_devs).
    :param title: Title for the plot.
    :param xlabel: Label for the x-axis.
    :param ylabel: Label for the y-axis.
    """
    plt.figure()
    log_nep = [math.log10(n) for n in nep_values]
    for label, (means, std_devs) in results.items():
        plt.errorbar(log_nep,
                     means,
                     yerr=std_devs,
                     label=str(label),
                     fmt='-o',
                     capsize=5)

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.savefig(title.replace(" ", "_") + ".png")


if __name__ == "__main__":
    num_tests = 10
    num_test_episodes = 1000
    discount = 0.95
    delta = 1 - discount
    alpha_init = 0.9
    alpha_final = 0.01

    nep_values = [10, 100, 1000, 10000, 50000, 100000]
    eps = [0.01, 0.1, 0.4]
    n_values_q3 = [2, 4, 10]
    n_values_q4 = [2, 3, 5]
    n_value_q5 = 1
    epsilon_q345 = 0.1

    print('q2')
    grid_name_q2 = 'fourroom'
    grid_q2 = get_grid(grid_name_q2)
    num_signals_q2 = 4
    results_q2 = {}

    for epsilon in eps:
        avg_rewards_eps = []
        std_devs_eps = []

        for nep in nep_values:
            print(f"{nep=}")
            test_rewards_all_tests = []
            for test in range(num_tests):
                sender = Sender(num_signals_q2, len(grid_q2), len(grid_q2[0]),
                                alpha_init, alpha_final, nep, epsilon, 0.0)
                receiver = Receiver(num_signals_q2, len(grid_q2),
                                    len(grid_q2[0]), alpha_init, alpha_final,
                                    nep, epsilon, discount)

                run_episodes(sender, receiver, grid_q2, nep, delta)

                sender_test = deepcopy(sender)
                receiver_test = deepcopy(receiver)
                sender_test.epsilon = 0.0
                sender_test.alpha = 0.0
                receiver_test.epsilon = 0.0
                receiver_test.alpha = 0.0

                test_rewards = run_episodes(sender_test, receiver_test, grid_q2,
                                            num_test_episodes, delta)
                test_rewards_all_tests.append(np.average(test_rewards))

            avg_rewards_eps.append(np.mean(test_rewards_all_tests))
            std_devs_eps.append(np.std(test_rewards_all_tests))

        results_q2[f"ε={epsilon}"] = (avg_rewards_eps, std_devs_eps)

    plot_results(nep_values, results_q2,
                 "Q2: Four Rooms - Avg Reward vs log(Nep)")

    print("q3")
    grid_name_q3 = 'fourroom'
    grid_q3 = get_grid(grid_name_q3)
    results_q3 = {}

    for n_sig in n_values_q3:
        print(f"{n_sig=}")
        avg_rewards_n = []
        std_devs_n = []

        for nep in nep_values:
            print(f"{nep=}")
            test_rewards_all_tests = []
            for test in range(num_tests):
                sender = Sender(n_sig, len(grid_q3), len(grid_q3[0]),
                                alpha_init, alpha_final, nep, epsilon_q345, 0.0)
                receiver = Receiver(n_sig, len(grid_q3), len(grid_q3[0]),
                                    alpha_init, alpha_final, nep, epsilon_q345,
                                    discount)
                run_episodes(sender, receiver, grid_q3, nep, delta)
                sender_test = deepcopy(sender)
                receiver_test = deepcopy(receiver)
                sender_test.epsilon = 0.0
                sender_test.alpha = 0.0
                receiver_test.epsilon = 0.0
                receiver_test.alpha = 0.0
                test_rewards = run_episodes(sender_test, receiver_test, grid_q3,
                                            num_test_episodes, delta)
                test_rewards_all_tests.append(np.average(test_rewards))

            avg_rewards_n.append(np.mean(test_rewards_all_tests))
            std_devs_n.append(np.std(test_rewards_all_tests))

        results_q3[f"N={n_sig}"] = (avg_rewards_n, std_devs_n)

    plot_results(nep_values, results_q3,
                 "Q3: Four Rooms - Avg Reward vs log(Nep) (ε=0.1)")

    print("q4")
    grid_name_q4 = 'maze'
    grid_q4 = get_grid(grid_name_q4)
    results_q4 = {}

    for n_sig in n_values_q4:
        print(f"{n_sig=}")
        avg_rewards_n = []
        std_devs_n = []

        for nep in nep_values:
            print(f"{nep=}")
            test_rewards_all_tests = []
            for test in range(num_tests):
                sender = Sender(n_sig, len(grid_q4), len(grid_q4[0]),
                                alpha_init, alpha_final, nep, epsilon_q345, 0.0)
                receiver = Receiver(n_sig, len(grid_q4), len(grid_q4[0]),
                                    alpha_init, alpha_final, nep, epsilon_q345,
                                    discount)
                run_episodes(sender, receiver, grid_q4, nep, delta)
                sender_test = deepcopy(sender)
                receiver_test = deepcopy(receiver)
                sender_test.epsilon = 0.0
                sender_test.alpha = 0.0
                receiver_test.epsilon = 0.0
                receiver_test.alpha = 0.0
                test_rewards = run_episodes(sender_test, receiver_test, grid_q4,
                                            num_test_episodes, delta)
                test_rewards_all_tests.append(np.average(test_rewards))

            avg_rewards_n.append(np.mean(test_rewards_all_tests))
            std_devs_n.append(np.std(test_rewards_all_tests))

        results_q4[f"N={n_sig}"] = (avg_rewards_n, std_devs_n)

    plot_results(nep_values, results_q4,
                 "Q4: Maze - Avg Reward vs log(Nep) (ε=0.1)")

    print("q5")
    grid_name_q5 = 'empty'
    grid_q5 = get_grid(grid_name_q5)
    results_q5 = {}
    n_sig_q5 = n_value_q5
    print(f"{n_sig_q5=}")
    avg_rewards_n = []
    std_devs_n = []

    for nep in nep_values:
        print(f"{nep=}")
        test_rewards_all_tests = []
        for test in range(num_tests):
            sender = Sender(n_sig_q5, len(grid_q5), len(grid_q5[0]), alpha_init,
                            alpha_final, nep, epsilon_q345, 0.0)
            receiver = Receiver(n_sig_q5, len(grid_q5), len(grid_q5[0]),
                                alpha_init, alpha_final, nep, epsilon_q345,
                                discount)
            run_episodes(sender, receiver, grid_q5, nep, delta)
            sender_test = deepcopy(sender)
            receiver_test = deepcopy(receiver)
            sender_test.epsilon = 0.0
            sender_test.alpha = 0.0
            receiver_test.epsilon = 0.0
            receiver_test.alpha = 0.0
            test_rewards = run_episodes(sender_test, receiver_test, grid_q5,
                                        num_test_episodes, delta)
            test_rewards_all_tests.append(np.average(test_rewards))

        avg_rewards_n.append(np.mean(test_rewards_all_tests))
        std_devs_n.append(np.std(test_rewards_all_tests))

    results_q5[f"N={n_sig_q5}"] = (avg_rewards_n, std_devs_n)

    plot_results(nep_values, results_q5,
                 "Q5: Empty - Avg Reward vs log(Nep) (ε=0.1, N=1)")
