import requests
from bs4 import BeautifulSoup
cnt = 0

def get_url(url, visited) :
	global cnt
	if url in visited :
		return
	else :
		try :
			r = requests.get(url)
			if r.ok == False :
				return
			visited.append(url)
			soup = BeautifulSoup(r.content, "html.parser")
			file_name = "Output_" + "%04d" % (cnt+1) + ".txt"
			fp = open(file_name, "w")
			fp.writelines(soup.get_text())
			fp.close()
			cnt = cnt + 1
			results = soup.find_all('a')
			for i in results :
				address = str(i)
				address = i.get('href')
				if(address != "" and '?' not in address and '#' not in address) :
					if(address.find("http://cspro.sogang.ac.kr/~gr120170213/") == -1) :
						address = 'http://cspro.sogang.ac.kr/~gr120170213/' + address;
					get_url(address, visited)
		except :
			pass

visited = []
get_url("http://cspro.sogang.ac.kr/~gr120170213/index.html", visited)
fp = open("URL.txt", "w")
for i in range(len(visited)) :
	fp.writelines(visited[i])
	fp.write("\n")
fp.close()
