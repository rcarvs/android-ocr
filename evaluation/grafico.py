"""
========
Barchart
========

A bar plot with errorbars and height labels on individual bars
"""
import numpy as np
import matplotlib.pyplot as plt
import os
import string
pasta = "/home/labpi/Documentos/renan/projetos/OpticalCharacterRecognitionParallel/evaluation/testes"
caminhos = os.listdir(pasta)
chaves = {
	'Program Initialization',
	'Task blackAndWhite - Buffers Initialization',
	'Task blackAndWhite - Task Configuration',
	'Task blackAndWhite - Task submition',
	'Task blackAndWhite - Runtime Finish',
	'Task blackAndWhite - Result Data Copy',
	'Cpp Create Dumbly Labels',
	'Matrix Letters Search - Sum',
	'Matrix Letters Search - Mean',
	'Task evaluation_and_crossing - Sum Buffers',
	'Task evaluation_and_crossing - Sum Tasks',
	'Task evaluation_and_crossing - Sum Submission',
	'Task evaluation_and_crossing - Sum Finish',
	'Task evaluation_and_crossing - Sum Result',
	'Task evaluation_and_crossing - Mean Buffers',
	'Task evaluation_and_crossing - Mean Tasks',
	'Task evaluation_and_crossing - Mean Submission',
	'Task evaluation_and_crossing - Mean Finish',
	'Task evaluation_and_crossing - Mean Result'
}
tamanhos = {
	'100_100',
	'500_500',
	'1000_1000',
}
for chave in chaves:
	chaves[chave] = {}
	for tamanho in tamanhos:
		chaves[chave][tamanho] = {}
		for caminho in caminhos:
			if(!chaves[chave][tamanho].has_key(int(caminho.replace(tamanho+'_','')))):
				chaves[chave][tamanho][int(caminho.replace(tamanho+'_',''))] = []
			file = open(pasta+'/'+caminho,'r')
			mean = []			
			for linha in file.readlines()
				saporra = string.split(linha,':')
				if(int(editdistance.eval(saporra[0],chave)) < 3):
					#vou considerar como igual
					mean.append(float(saporra[1].strip()))
			mean_result = numpy.array()
						



dicionario = {}
for caminho in caminhos:
	file = open(pasta+'/'+caminho,'r')
	for linha in file.readlines():
		saporra = string.split(linha,':')
		if(dicionario.has_key(saporra[0])):
			dicionario[saporra[0]].append(float(saporra[1].strip()))
		else:
			lista = []
			dicionario[saporra[0]] = lista
			dicionario[saporra[0]].append(float(saporra[1].strip()))	
	file.close();




N = 5
cem_por_cem = (20, 35, 30, 35, 27)
men_std = (2, 3, 4, 1, 2)

ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(ind, men_means, width, color='r', yerr=men_std)

women_means = (25, 32, 34, 20, 25)
women_std = (3, 5, 2, 3, 3)
rects2 = ax.bar(ind + width, women_means, width, color='y', yerr=women_std)

# add some text for labels, title and axes ticks
ax.set_ylabel('Scores')
ax.set_title('Scores by group and gender')
ax.set_xticks(ind + width / 2)
ax.set_xticklabels(('1', '10', '50', '100', '400','800','1000','4000','5000','8000','16000'))

ax.legend((rects1[0], rects2[0]), ('Men', 'Women'))


def autolabel(rects):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom')

autolabel(rects1)
autolabel(rects2)

plt.show()