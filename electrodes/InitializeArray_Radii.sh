#!/bin/bash -l
#SBATCH --job-name="EEG_2_CoordsV"
#SBATCH --partition=prod
#SBATCH --nodes=1
#SBATCH -C clx
#SBATCH --cpus-per-task=2
#SBATCH --time=2:00:00
##SBATCH --mail-type=ALL
#SBATCH --account=proj85
#SBATCH --no-requeue
#SBATCH --output=EEG_sonata_CoordsV.out
#SBATCH --error=EEG_sonata_CoordsV.err
#SBATCH --exclusive
#SBATCH --mem=0

spack env activate bluerecording-dev
source ~/bluerecording-dev/bin/activate

srun -n 1 python run_initialize_h5.py 'electrode_csv_radii.csv' 'simulation_config.json' 'coeffs_radii.h5' 
