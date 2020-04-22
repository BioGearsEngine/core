# Patient Geneation
This goal of this example is to make a howto with a variable timeline that will be used to quickly generate a network of patient states for dataminning.  The HowTo takes command line arguments and generates a log and csv file with updates occuring every five minutes.  A data miner can then process each csv file to determine if it produces statistically signifcant differences to be used for training a decision network.

In this HowTo a veradic scenario is run based on the command line arguments. The scenario is the following

1. Patient Infection of [severity] with a MIC [strength] occurs at 5 minutes after start
2. Patient receives antibiotic treatment at [X] minutes and every [interval] minutes there after
3. Scenario runs for a total of [maximum] hours 

You can run up [N] scenarios over [J] threads each thread will need its own set of arguents.  The device can read a csv file to simplify the input

Example -j 5 --duration 5 --trials "1,1,60,60" "1,2,60,60" "1,1,30,30" "1,2,120,120" "3,3,120,120"
## Arguments
| Name     |  Syntax | Purpose | Example | Default | 
|----------|---------|---------|---------|---------|
| Thread Count | -j[N] | Set the maximum number of threads | -j 4 | Total - 1 |
| Duration | --duration [N] | Set the Initial Infection level | --duration 5 | 6 horus |  
| INPUTS| --trials "X,Y,Z[,t]" | MIC, First Treatment Occurance, Interval Between Treatments[,optional  Total Duration] | --trials "1,1,60,60" "1,2,60,60" | None | 
| CONFIG | --config [file] | Configure trial inputs with a csv file | --config trials.csv |


## CSV FORMAT

Simple CSV FILE
```
Duration, [Thread Pool]
MIC, First Treatment, Interval for treatments, [Duration]
MIC, First Treatment, Interval for treatments, [Duration]
MIC, First Treatment, Interval for treatments, [Duration]
MIC, First Treatment, Interval for treatments, [Duration]
MIC, First Treatment, Interval for treatments, [Duration]

```
## TODO 
Make Initial Severity Variable

