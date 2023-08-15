import numpy as np
import pandas as pd
import sys

#if (len(sys.argv) != 2):
#	print(f"Argument error")
#	exit() 

#filepath = './lyney_the_first_great_magic_set.csv'
#filepath = filepath.replace('set', sys.argv[1])
#dataset = pd.read_csv('./lyney_the_first_great_magic_{sys.argv[1]}.csv')
#dataset = pd.read_csv(filepath)

#values_list = []

def run(dataset, artifact_set):
	values_list = []

	for index, row in dataset.iterrows():
		for i in range(row['SAMPLES']):
			values_list.append(row['DMG_VALUE'])
		
	dmg_df = pd.DataFrame(values_list)
	print(f'{artifact_set}:')
	print(f'Mean: {dmg_df.mean()}')
	print(f'Std dev: {dmg_df.std()}')
	print(f'--------------\n')

run(pd.read_csv('./lyney_the_first_great_magic_mh.csv'), 'MARECHUSSEE HUNTER')
run(pd.read_csv('./lyney_the_first_great_magic_lw.csv'), 'LAVAWALKER')
run(pd.read_csv('./lyney_the_first_great_magic_sr.csv'), 'SHIMENAWA')
run(pd.read_csv('./lyney_the_first_great_magic_wt.csv'), 'WANDERER TROUPE')
