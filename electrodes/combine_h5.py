#!/usr/bin/env python3
"""
Script to combine two h5 files with the same structure.
- Preserves S1nonbarrel_neurons group (assumed identical in both files)
- Concatenates scaling_factors datasets from electrodes/S1nonbarrel_neurons
- Copies electrode groups 0-99 from first file
- Copies electrode groups 0-99 from second file and renames to 100-199
- Renumbers the integer stored in electrodes/<n>/S1nonbarrel_neurons to match
  the new group name
"""

import h5py
import numpy as np
import sys
from pathlib import Path


def combine_h5_files(file1_path, file2_path, output_path, chunk_size=100000):
    """
    Combine two h5 files with the same structure.
    
    Parameters:
    -----------
    file1_path : str
        Path to first h5 file (coeffs_highRes_oCSD_only.h5)
    file2_path : str
        Path to second h5 file (coeffs_highRes_lfpOnly.h5)
    output_path : str
        Path for the combined output file
    chunk_size : int, optional
        Number of rows to process at a time (default: 100000)
        Adjust this based on available memory
    """
    
    print(f"Combining {file1_path} and {file2_path}")
    print(f"Output will be saved to {output_path}")
    
    # Open both input files in read mode
    with h5py.File(file1_path, 'r') as f1, h5py.File(file2_path, 'r') as f2:
        
        # Create output file
        with h5py.File(output_path, 'w') as out:
            
            # 1. Copy S1nonbarrel_neurons group from file1 (assumed identical in both)
            print("\nCopying top-level S1nonbarrel_neurons group...")
            f1.copy('S1nonbarrel_neurons', out)
            
            # 2. Create electrodes group
            print("Creating electrodes group...")
            electrodes_group = out.create_group('electrodes')
            
            # 3. Copy electrode groups 0-99 from file1 (no renumbering needed)
            print("Copying electrode groups 0-99 from first file...")
            for i in range(100):
                group_name = str(i)
                if group_name in f1['electrodes']:
                    f1['electrodes'].copy(group_name, electrodes_group)
                    print(f"  Copied electrode group {i}", end='\r')
            print(f"\nCompleted copying groups 0-99 from first file")
            
            # 4. Copy electrode groups 0-99 from file2, rename to 100-199,
            #    and renumber the integer inside each group's S1nonbarrel_neurons
            print("Copying electrode groups 0-99 from second file (renaming to 100-199)...")
            for i in range(100):
                old_name = str(i)
                new_name = str(i + 100)
                if old_name in f2['electrodes']:
                    # Copy the group under the new name
                    f2['electrodes'].copy(old_name, electrodes_group, name=new_name)
                    
                    # Overwrite the integer inside S1nonbarrel_neurons to match new index
                    s1_path = f'{new_name}/S1nonbarrel_neurons'
                    if s1_path in electrodes_group:
                        electrodes_group[s1_path][()] = i + 100
                    
                    print(f"  Copied electrode group {i} -> {new_name}", end='\r')
            print(f"\nCompleted copying groups 0-99 from second file as 100-199")
            
            # 5. Concatenate scaling_factors from both files (chunked to avoid memory overload)
            print("\nConcatenating scaling_factors datasets...")
            
            sf1_path = 'electrodes/S1nonbarrel_neurons/scaling_factors'
            sf2_path = 'electrodes/S1nonbarrel_neurons/scaling_factors'
            
            if sf1_path in f1 and sf2_path in f2:
                sf1_dataset = f1[sf1_path]
                sf2_dataset = f2[sf2_path]
                
                print(f"  First file scaling_factors shape: {sf1_dataset.shape}")
                print(f"  Second file scaling_factors shape: {sf2_dataset.shape}")
                
                # Verify that both datasets have the same number of rows
                if sf1_dataset.shape[0] != sf2_dataset.shape[0]:
                    raise ValueError(f"Row dimension mismatch: {sf1_dataset.shape[0]} vs {sf2_dataset.shape[0]}")
                
                n_rows = sf1_dataset.shape[0]
                n_cols_combined = sf1_dataset.shape[1] + sf2_dataset.shape[1]
                
                print(f"  Combined scaling_factors shape: ({n_rows}, {n_cols_combined})")
                
                # Create S1nonbarrel_neurons group in electrodes
                s1_group = electrodes_group.create_group('S1nonbarrel_neurons')
                
                # Create output dataset with combined shape
                sf_combined = s1_group.create_dataset(
                    'scaling_factors', 
                    shape=(n_rows, n_cols_combined),
                    dtype=sf1_dataset.dtype,
                    compression='gzip', 
                    compression_opts=4
                )
                
                # Process in chunks to avoid memory overload
                n_chunks = (n_rows + chunk_size - 1) // chunk_size
                
                print(f"  Processing in {n_chunks} chunks of up to {chunk_size:,} rows...")
                
                for i in range(n_chunks):
                    start_idx = i * chunk_size
                    end_idx = min((i + 1) * chunk_size, n_rows)
                    
                    # Read chunks from both datasets
                    chunk1 = sf1_dataset[start_idx:end_idx, :]
                    chunk2 = sf2_dataset[start_idx:end_idx, :]
                    
                    # Concatenate chunks along axis 1 (columns)
                    chunk_combined = np.concatenate([chunk1, chunk2], axis=1)
                    
                    # Write to output dataset
                    sf_combined[start_idx:end_idx, :] = chunk_combined
                    
                    progress = (i + 1) / n_chunks * 100
                    print(f"    Progress: {progress:.1f}% (rows {start_idx:,} to {end_idx:,})", end='\r')
                
                print(f"\n  Successfully concatenated scaling_factors dataset")
            else:
                print("  WARNING: scaling_factors not found in expected location")
                if sf1_path not in f1:
                    print(f"    Missing in file1: {sf1_path}")
                if sf2_path not in f2:
                    print(f"    Missing in file2: {sf2_path}")
    
    print(f"\nSuccessfully created combined file: {output_path}")


def main():
    """Main function to handle command line arguments."""
    
    if len(sys.argv) < 4 or len(sys.argv) > 5:
        print("Usage: python combine_h5_files.py <file1.h5> <file2.h5> <output.h5> [chunk_size]")
        print("\nArguments:")
        print("  file1.h5    : First input h5 file")
        print("  file2.h5    : Second input h5 file")
        print("  output.h5   : Output combined h5 file")
        print("  chunk_size  : (Optional) Number of rows to process at once (default: 100000)")
        print("\nExample:")
        print("  python combine_h5_files.py coeffs_highRes_oCSD_only.h5 coeffs_highRes_lfpOnly.h5 combined_coeffs.h5")
        print("  python combine_h5_files.py file1.h5 file2.h5 output.h5 50000")
        sys.exit(1)
    
    file1_path = sys.argv[1]
    file2_path = sys.argv[2]
    output_path = sys.argv[3]
    chunk_size = int(sys.argv[4]) if len(sys.argv) == 5 else 100000
    
    # Check if input files exist
    if not Path(file1_path).exists():
        print(f"Error: File not found: {file1_path}")
        sys.exit(1)
    
    if not Path(file2_path).exists():
        print(f"Error: File not found: {file2_path}")
        sys.exit(1)
    
    # Check if output file already exists
    if Path(output_path).exists():
        response = input(f"Warning: {output_path} already exists. Overwrite? (y/n): ")
        if response.lower() != 'y':
            print("Aborted.")
            sys.exit(0)
    
    # Combine the files
    print(f"Using chunk size: {chunk_size:,} rows")
    combine_h5_files(file1_path, file2_path, output_path, chunk_size)


if __name__ == "__main__":
    main()
