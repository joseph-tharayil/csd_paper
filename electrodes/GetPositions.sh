#!/bin/bash -l
#SBATCH --job-name="EEG_1_CoordsV"
#SBATCH --partition=prod
#SBATCH --nodes=142
#SBATCH -C clx
#SBATCH --cpus-per-task=2
#SBATCH --time=24:00:00
##SBATCH --mail-type=ALL
#SBATCH --account=proj85
#SBATCH --no-requeue
#SBATCH --output=EEG_1_CoordsV.out
##SBATCH --error=EEG_1_CoordsV.err
#SBATCH --exclusive
#SBATCH --mem=0

spack env activate bluerecording-dev
source ~/bluerecording-dev/bin/activate

NEURONS_PER_FILE=1000

FILES_PER_FOLDER=50

for i in {0..4235}
do

    folder="positions/$(($i/$FILES_PER_FOLDER))"
    mkdir -p $folder 2>/dev/null

done

srun -n 4235 python run_get_positions.py "simulation_config.json" "positions" $NEURONS_PER_FILE $FILES_PER_FOLDER

