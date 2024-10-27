#!/bin/bash -l
#SBATCH --job-name="EEG_2_CoordsV"
#SBATCH --partition=prod
#SBATCH --nodes=1
#SBATCH --time=2:00:00
#SBATCH --account=proj83
#SBATCH -C clx
#SBATCH --cpus-per-task=2
#SBATCH --mem=0

module load unstable py-mpi4py
source ~/bluerecording-dev/bin/activate

srun -n 1 python writeCSV_notSphere.py 'Neuropixels-384' 'simulation_config.json' 'electrode_csv_notSphere.csv'
