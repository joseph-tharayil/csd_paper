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
#SBATCH --output=EEG_0_CoordsV.out
#SBATCH --error=EEG_0_CoordsV.err
#SBATCH --exclusive
#SBATCH --mem=0


#spack load py-bluepy/jrzr2b
#module load unstable py-bluepy py-mpi4py

module load unstable hpe-mpi py-scikit-learn py-bluepy
#source ~/bluepy-env/bin/activate

srun -n 1 python eegTotal_perTrial.py
