#### A dynamic programming algorithm for the Robotic Cell Problem with job-independent processing times

Alessandro Minoli, Giovanni Righini (2025)

##### Usage

Compile and run on a specific instance with :

```
mkdir build
cd build
cmake ..
make
cd ..
./bin/RCP_with_job-independent_processing_times <path_to_instance_file>
```

Test on a dataset of instances with :

```
./run_on_dataset.sh
```

##### Format of instance files

```
J                                   # number of jobs
M                                   # number of machines
p_1  p_2  ...  p_M                  # processing time on each machine
t_0,0    t_0,1    ...  t_0,M+1      # travel time between each pair of stations
t_1,0    t_1,1    ...  t_1,M+1
...
t_M+1,0  t_M+1,1  ...  t_M+1,M+1
```