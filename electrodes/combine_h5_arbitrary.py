#!/usr/bin/env python3
"""
Script to combine two h5 files with the same structure.
- Preserves S1nonbarrel_neurons group (assumed identical in both files)
- Concatenates scaling_factors datasets from electrodes/S1nonbarrel_neurons
- Copies all integer-named electrode groups from first file (unchanged)
- Copies all integer-named electrode groups from second file, offset by the
  number of electrode groups in the first file, and renumbers the integer
  stored in each group's S1nonbarrel_neurons to match the new group name
"""

import h5py
import numpy as np
import sys
from pathlib import Path


def get_electrode_indices(f):
    """
    Return a sorted list of integer-named keys in f['electrodes'],
    excluding non-integer keys like 'S1nonbarrel_neurons'.
    """
    keys = []
    for key in f['electrodes'].keys():
        try:
            keys.append(int(key))
        except ValueError:
            pass
    return sorted(keys)


def combine_h5_files(file1_path, file2_path, output_path, chunk_size=100000):
    """
    Combine two h5 files with the same structure.

    Parameters:
    -----------
    file1_path : str
        Path to first h5 file
    file2_path : str
        Path to second h5 file
    output_path : str
        Path for the combined output file
    chunk_size : int, optional
        Number of rows to process at a time (default: 100000)
        Adjust this based on available memory
    """

    print(f"Combining {file1_path} and {file2_path}")
    print(f"Output will be saved to {output_path}")

    with h5py.File(file1_path, 'r') as f1, h5py.File(file2_path, 'r') as f2:

        indices_f1 = get_electrode_indices(f1)
        indices_f2 = get_electrode_indices(f2)
        n_f1 = len(indices_f1)
        n_f2 = len(indices_f2)
        print(f"\nFound {n_f1} electrode group(s) in first file:  {indices_f1}")
        print(f"Found {n_f2} electrode group(s) in second file: {indices_f2}")

        # The offset applied to every group from file2
        offset = n_f1

        with h5py.File(output_path, 'w') as out:

            # 1. Copy S1nonbarrel_neurons group from file1 (assumed identical)
            print("\nCopying top-level S1nonbarrel_neurons group...")
            f1.copy('S1nonbarrel_neurons', out)

            # 2. Create electrodes group
            print("Creating electrodes group...")
            electrodes_group = out.create_group('electrodes')

            # 3. Copy all electrode groups from file1 (no renumbering needed)
            print(f"Copying {n_f1} electrode group(s) from first file...")
            for i, idx in enumerate(indices_f1):
                group_name = str(idx)
                f1['electrodes'].copy(group_name, electrodes_group)
                print(f"  Copied electrode group {idx} ({i + 1}/{n_f1})", end='\r')
            print(f"\n  Done.")

            # 4. Copy all electrode groups from file2, offset indices, and
            #    renumber the integer inside each group's S1nonbarrel_neurons
            print(f"Copying {n_f2} electrode group(s) from second file "
                  f"(offset +{offset})...")
            for i, idx in enumerate(indices_f2):
                old_name = str(idx)
                new_idx = idx + offset
                new_name = str(new_idx)

                f2['electrodes'].copy(old_name, electrodes_group, name=new_name)

                # Overwrite the integer inside S1nonbarrel_neurons to match new index
                s1_path = f'{new_name}/S1nonbarrel_neurons'
                if s1_path in electrodes_group:
                    electrodes_group[s1_path][()] = new_idx

                print(f"  Copied electrode group {idx} -> {new_name} "
                      f"({i + 1}/{n_f2})", end='\r')
            print(f"\n  Done.")

            # 5. Concatenate scaling_factors from both files (chunked)
            print("\nConcatenating scaling_factors datasets...")

            sf_path = 'electrodes/S1nonbarrel_neurons/scaling_factors'

            if sf_path in f1 and sf_path in f2:
                sf1 = f1[sf_path]
                sf2 = f2[sf_path]

                print(f"  First file  scaling_factors shape: {sf1.shape}")
                print(f"  Second file scaling_factors shape: {sf2.shape}")

                if sf1.shape[0] != sf2.shape[0]:
                    raise ValueError(
                        f"Row dimension mismatch: {sf1.shape[0]} vs {sf2.shape[0]}"
                    )

                n_rows = sf1.shape[0]
                n_cols_sf1 = sf1.shape[1] - 1  # ignore last column of sf1
                n_cols_combined = n_cols_sf1 + sf2.shape[1]
                print(f"  First file  scaling_factors used columns: {n_cols_sf1} (last column ignored)")
                print(f"  Combined    scaling_factors shape: ({n_rows}, {n_cols_combined})")

                s1_group = electrodes_group.create_group('S1nonbarrel_neurons')
                sf_combined = s1_group.create_dataset(
                    'scaling_factors',
                    shape=(n_rows, n_cols_combined),
                    dtype=sf1.dtype,
                    compression='gzip',
                    compression_opts=4,
                )

                n_chunks = (n_rows + chunk_size - 1) // chunk_size
                print(f"  Processing in {n_chunks} chunk(s) of up to {chunk_size:,} rows...")

                for chunk_idx in range(n_chunks):
                    start = chunk_idx * chunk_size
                    end = min(start + chunk_size, n_rows)

                    chunk_combined = np.concatenate(
                        [sf1[start:end, :-1], sf2[start:end, :]], axis=1
                    )
                    sf_combined[start:end, :] = chunk_combined

                    progress = (chunk_idx + 1) / n_chunks * 100
                    print(f"    Progress: {progress:.1f}% "
                          f"(rows {start:,} – {end:,})", end='\r')

                print(f"\n  Successfully concatenated scaling_factors dataset.")
            else:
                print("  WARNING: scaling_factors not found in expected location.")
                if sf_path not in f1:
                    print(f"    Missing in file1: {sf_path}")
                if sf_path not in f2:
                    print(f"    Missing in file2: {sf_path}")

    print(f"\nSuccessfully created combined file: {output_path}")


def main():
    if len(sys.argv) < 4 or len(sys.argv) > 5:
        print("Usage: python combine_h5_files.py <file1.h5> <file2.h5> <output.h5> [chunk_size]")
        print("\nArguments:")
        print("  file1.h5    : First input h5 file")
        print("  file2.h5    : Second input h5 file")
        print("  output.h5   : Output combined h5 file")
        print("  chunk_size  : (Optional) Number of rows to process at once (default: 100000)")
        print("\nExample:")
        print("  python combine_h5_files.py a.h5 b.h5 combined.h5")
        print("  python combine_h5_files.py a.h5 b.h5 combined.h5 50000")
        sys.exit(1)

    file1_path = sys.argv[1]
    file2_path = sys.argv[2]
    output_path = sys.argv[3]
    chunk_size = int(sys.argv[4]) if len(sys.argv) == 5 else 100000

    for path in (file1_path, file2_path):
        if not Path(path).exists():
            print(f"Error: File not found: {path}")
            sys.exit(1)

    if Path(output_path).exists():
        response = input(f"Warning: {output_path} already exists. Overwrite? (y/n): ")
        if response.lower() != 'y':
            print("Aborted.")
            sys.exit(0)

    print(f"Using chunk size: {chunk_size:,} rows")
    combine_h5_files(file1_path, file2_path, output_path, chunk_size)


if __name__ == "__main__":
    main()
