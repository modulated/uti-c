# Bioseq Module
Parses and analyses biological sequences such as DNA and protein  strands.

## Data

### `bioseq`
Struct containing the sequence, length and type of sequence.

###  `bioseq_sequence_type`
Either `SEQUENCE_NONE`, `SEQUENCE_RNA`, `SEQUENCE_DNA` or `SEQUENCE_PROTEIN`

## Functions

### `bioseq bioseq_new(sequence_type type, char seq[])`

### `void bioseq_delete(bioseq seq)`

### `bioseq bioseq_reverse(bioseq seq)`

### `bioseq bioseq_complement(bioseq seq)`

### `bioseq bioseq_dna_protein(bioseq dna, int offset)`

### `void bioseq_protein_interactions(bioseq seq)`

