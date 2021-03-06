/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef parallelus_SCHEDULER_HPP
#define parallelus_SCHEDULER_HPP

#include <vector>

namespace parallelus {
class Device;
class Task;

/**
 * The Scheduler class specifies an interface that is used by all the different
 * scheduling policies implemented by the Runtime. New scheduling policies can
 * also be created by deriving this class and using the derived class when
 * constructing a Runtime class instance.
 *
 * @author Renato Utsch
 */
class Scheduler {

public:
    Scheduler() = default;
    Scheduler(const Scheduler &) = delete;
    Scheduler &operator=(const Scheduler &) = delete;

    virtual ~Scheduler() = default;

    /**
     * Pushes a task into the scheduler.
     * This function is thread-safe.
     */
    virtual void push(std::unique_ptr<Task> task) = 0;

    /**
     * Pops a task from the scheduler.
     * This function is thread-safe.
     */
    virtual std::unique_ptr<Task> pop(Device &device) = 0;

    /**
     * Waits until the scheduler doesn't have any more work to do.
     * This function is thread-safe.
     */
    virtual void waitUntilIdle() = 0;
    
    virtual std::unique_ptr<Task> netpop() = 0;

    virtual bool workersBusy(int busyRate) = 0;
};

}

#endif // !parallelus_SCHEDULER_HPP
