# Bioseq Module
Parses and analyses biological sequences such as DNA and protein.

## Data Structures

### `bioseq_dna`
Struct containing the sequence and length of DNA.

### `bioseq_protein`
Struct containing the sequence and length of protein.

### `bioseq_frame`
Struct containing six `bioseq_protein` structs for all reading frames. 

## Functions

### `bioseq bioseq_dna_construct(char seq[])`
Create a DNA sequence struct from string.

### `bioseq bioseq_protein_construct(char seq[])`
Create a protein sequence struct from string.

### `void bioseq_dna_destruct(bioseq seq)`
Cleanup DNA struct after use.

### `void bioseq_protein_destruct(bioseq seq)`
Cleanup protein struct after use.

### `bioseq_dna bioseq_dna_reverse(bioseq seq)`
Returns reverse sequence of DNA.

### `bioseq_dna bioseq_dna_complement(bioseq seq)`
Returns complement sequence of DNA.

### `bioseq_protein bioseq_dna_protein(bioseq dna, int offset)`
Convert DNA sequence to protein sequence, starting at `offset`.

### `void bioseq_protein_interactions(bioseq seq)`

