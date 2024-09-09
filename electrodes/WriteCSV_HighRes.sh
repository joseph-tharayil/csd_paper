#!/bin/bash -l
#SBATCH --job-name="EEG_2_CoordsV"
#SBATCH --partition=prod_small
#SBATCH --nodes=1
#SBATCH --time=2:00:00
##SBATCH --mail-type=ALL
#SBATCH --account=proj83
#SBATCH --no-requeue
#SBATCH --output=EEG_2_CoordsV.out
#SBATCH --error=EEG_2_CoordsV.err

module load unstable py-mpi4py
source ~/bluerecording-dev/bin/activate

srun -n 1 python writeCSV_highRes.py 'Neuropixels-384' 'simulation_config.json' 'electrode_csv_highRes.csv'
