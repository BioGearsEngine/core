# Patient Geneation
This HowTo Demonstrates how to batch run multiple physiology-engines within a thread-pool. The HowTo takes in
a series of command line arguments to determine the paramaters of multiple trials which will use the trails individual parmaters
to determine the individual behavior of the trial.

## Motivation
Combining Physics simulations results with ML systems as training data is a way to push back a common lack of common data reguarding 
specific human adversities due to the lack of ethical ways of collecting controll data.  In this scenario we generate a series of patients intended to train
a Hypvolemic Shock detector. A given trail will create a virtual patient with an initial infection then beign applying one of several treatment plans after a specific interval of time

### Treatments
 * NONE - No Treatment or Interventions Applied 
 * STANDARD - An infusion of Antibiotics wil start after a given interval of X hours and reapplied every Y minutes
 * REFRESH - If the patient  - REstricted Fluid REsuscitation in Sepsis-associated Hypotension[1]
 * EGDT - EGDT - Early Goal Directed Therapy
 * RANDOM - Select a Random choice of the above.

## Trial Treatment
In this HowTo a veradic scenario is run based on the command line arguments. The scenario is the following

1. Patient Infection of [severity] with a MIC [strength] occurs at 5 minutes after start
2. Patient receives antibiotic treatment at [X] minutes and every [interval] minutes there after
3. Scenario runs for a total of [maximum] hours 
4. Patient is feed every 8 hours.
5. TODO: Manual Patient Urination

You can run up [N] scenarios over [J] threads each thread will need its own set of arguents.  The device can read a csv file to simplify the input

Example -j 5 --duration 5 --trials "1,1,60,60" "1,2,60,60" "1,1,30,30" "1,2,120,120" "3,3,120,120"
## Arguments
| Name     |  Syntax | Purpose | Example | Default | 
|----------|---------|---------|---------|---------|
| Thread Count | -j[N] | Set the maximum number of threads | -j 4 | Total - 1 |
| Duration | --duration [N] | Set the Initial Infection level | --duration 5 | 6 horus |  
| INPUTS| --trials [ [PARAMS]... ] | Inline trial paramaters for multiple trials  | See (#Trial Parameters) | |
| CONFIG | --config [file] | Configure trial inputs with a csv file | --config trials.csv |

## Trial Parameters


| NAME | Optional | Type    |  Values  | Purpose |  Default    |
|------|----------|---------|----------|--------|-------|
| Severity | Required | Enum  | [None, Low, Moderate, Severe] | Sets the everity of the initial infection ||
| Treatment Plan | Required | Enum  | [None, Standard, REFRESH, EGDT, RANDOM] | Sets the everity of the initial infection ||
| MIC | Required | Real | mic_g_per_l | Minimal inhebitory concentration of the bacterial infection ||
| First treatment occurence | Required |  Integral | minutes | Delay Until first antibiotics is applied allowing time for infection to grow ||
| Interval between treatments | Required |  Integral | minutes | Number of minutes between antibiotic treatments|
| Patient files  | Optional |  Integral | hours | Specifcy which patient to load  | patient/StandardMale@0s.xml |
| Total Duration | Optional | Integral | hours | Length of full trial | (6) or --duration N |

 `Example --trials "SEVERE,STANDARD,1,1,60, States/StandardMale@0s, 60" "MODERATE,REFRESH,1,2,60"`

## CSV FORMAT

Simple CSV FILE
```
Duration, [Thread Pool]
Default Patient
Severity, PLAN, MIC, First Treatment, Interval for treatments, [Patient, Duration]
Severity, PLAN, MIC, First Treatment, Interval for treatments, [Patient, Duration]
Severity, PLAN, MIC, First Treatment, Interval for treatments, [Patient, Duration]
Severity, PLAN, MIC, First Treatment, Interval for treatments, [Patient, Duration]
Severity, PLAN, MIC, First Treatment, Interval for treatments, [Patient, Duration]
```

## Citations
 * McDaniel, Matthew, et al. "A Whole-Body Mathematical Model of Sepsis Progression and Treatment Designed in the BioGears Physiology Engine." Frontiers in physiology 10 (2019): 1321. https://www.frontiersin.org/articles/10.3389/fphys.2019.01321/full