#!/bin/bash -l
#SBATCH --job-name="EEG_2_CoordsV"
#SBATCH --partition=prod
#SBATCH --nodes=1
#SBATCH -C volta
#SBATCH --cpus-per-task=2
#SBATCH --time=24:00:00
##SBATCH --mail-type=ALL
#SBATCH --account=proj85
#SBATCH --no-requeue
#SBATCH --output=EEG_0a_CoordsV.out
#SBATCH --error=EEG_0a_CoordsV.err
#SBATCH --exclusive
#SBATCH --mem=0

module load unstable py-mpi4py
source ~/conntilitEnv/bin/activate

srun -n 1 python getCsdDistances_acrossConditions-scaled.py 1
srun -n 1 python getCsdDistances_acrossConditions-scaled.py .5
srun -n 1 python getCsdDistances_acrossConditions-scaled.py 2
