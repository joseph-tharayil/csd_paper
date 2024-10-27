#!/bin/bash -l
#SBATCH --job-name="EEG_2_CoordsV"
#SBATCH --partition=prod
#SBATCH --nodes=1
#SBATCH -C clx
#SBATCH --account=proj83
#SBATCH --output=EEG_2_CoordsV.out
#SBATCH --error=EEG_2_CoordsV.err
#SBATCH --mem=0

module load unstable py-mpi4py
source ~/bluerecording-dev/bin/activate

srun -n 1 python writeCSV_smallRadii.py 'Neuropixels-384' 'simulation_config.json' 'electrode_csv_smallRadii.csv'
