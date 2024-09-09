#!/bin/bash -l
#SBATCH --job-name="EEG_2_CoordsV"
#SBATCH --partition=prod
#SBATCH --nodes=10
#SBATCH -C clx
#SBATCH --cpus-per-task=2
#SBATCH --time=24:00:00
##SBATCH --mail-type=ALL
#SBATCH --account=proj85
#SBATCH --no-requeue
#SBATCH --output=EEG_0_CoordsV.out
#SBATCH --error=EEG_0_CoordsV.err
#SBATCH --exclusive
#SBATCH --mem=0


#spack load py-bluepy/jrzr2b
#module load unstable py-bluepy py-mpi4py

module load unstable py-bluepysnap py-bluepy

mkdir pkls
for i in {0..10}
do
    mkdir $i/pkls
done

srun -n 10 python geteeg.py

