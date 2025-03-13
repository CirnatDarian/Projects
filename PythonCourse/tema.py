"""biblioteca folosita pentru a trimite cereri catre URL specificat
   si de a extrage informatiile din pagina"""
import requests

"""biblioteca folosita pentru a parsa si naviga printre documentele HTML"""
from bs4 import BeautifulSoup
import configparser

"""biblioteca pentru a extrage pretul produselor """
import re
import time
import argparse
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

def get_page_info(url):
    """functie folosita pentru trimiterea unor cereri catre URL-ul
        dat de utilizator, extragerea informatiilor cerute din acesta
        si returnarea lor prin 2 obiecte
        parametrul1: url - primeste adresa url de la tastatura """
    response = requests.get(url)  # obtinem informatiile paginii

    # parsam continutul html
    soup = BeautifulSoup(response.text, 'html.parser')

    # extragem titlul si descrierea paginii
    title = soup.title.string.strip() if soup.title else "N/A"
    description = soup.find('meta', attrs={'name': 'description'}).get('content').strip() if soup.find('meta',
                                                                                                       attrs={
                                                                                                           'name': 'description'}) else "N/A"

    return title, description


"""se preiau datele introduse de utilizator del a tastatura"""
url = input("Introdu URL-ul paginii: ")

# retinem informatiile in 2 obiecte
title, description = get_page_info(url)

"""afisam datele obtinute sau un mesaj daca
datele nu au putut fi extrase"""
if title and description:
    print(f"\nTitlu paginii: {title}")
    print(f"Descriere: {description}")
else:
    print("Nu s-au putut obține informațiile de la pagină.")

"""exercitiul 2"""
"""creare obiect configparser"""
config = configparser.ConfigParser()

"""citire fisier .ini"""
config.read('config.ini')

"""obtinem URL-ul din fișier"""
url_1 = config.get('DEFAULT', 'url')

title_1, description_1 = get_page_info(url_1)

if title and description:
    print(f"\nTitlu paginii: {title_1}")
    print(f"Descriere: {description_1}")
else:
    print("Nu am putut obtine informatiile din pagina")

"""exercitiul 3"""
"""functie prin care extragem datele in functie de cuvantul cheie dat"""


def get_page_info_3(url, keyword):
    start = time.time()
    response = requests.get(f"{url}/search/q={keyword}")
    soup = BeautifulSoup(response.text, 'html.parser')
    ads = soup.find_all('div', {'data-cy': 'l-card'})
    info_titlu_pret = []

    for ad in ads:
        title = ad.find('div', {'data-cy': 'l-card'}).text if ad.find('div', {'data-cy': 'l-card'}) else "N/A"
        price = ad.find('p', {'data-testid': 'ad-price'}).text if ad.find('p', {'data-testid': 'ad-price'}) else "N/A"

        price = re.sub(r'\D', '', price)
        info_titlu_pret.append((title, price))

    # sortare
    info_titlu_pret.sort(key=lambda x: x[1])

    end = time.time()
    execution_time = end - start

    return info_titlu_pret, execution_time


"""parseaza argumentele pentru linia de comanda"""
parser = argparse.ArgumentParser()
parser.add_argument("-log", action="store_true")
args = parser.parse_args()

config = configparser.ConfigParser()
config.read('config.ini')
url_3 = config.get('DEFAULT', 'url')
keyword_3 = config.get('DEFAULT', 'keyword')

info_titlu_pret, execution_time = get_page_info_3(url_3, keyword_3)

if args.log:
    print(f"Timpul necesar pentru request: {execution_time} secunde")

for title, price in info_titlu_pret:
    print(f"Titlu anunt: {title}, Pret: {price}")

"""exercitiul 5"""
"""fc send_email trimite un email catre adresa specificata in fisierul .ini"""
def send_email(email, title, price):
    msg = MIMEMultipart()
    msg['From'] = 'stefandarian@yahoo.com'
    msg['To'] = email
    msg['Subject'] = 'Pretul a scazut!!'
    body = f"Pretul pentru {title} a scazut la {price}! Cumpara ACUM!"
    msg.attach(MIMEText(body, 'plain'))

    """conectare la server smtp, port 587"""
    server = smtplib.SMTP('smtp.example.com', 587)
    server.starttls()
    server.login('stefandarian@yahoo.com', 'your-password')
    text = msg.as_string()
    server.sendmail('stefandarian@yahoo.com', email, text)
    server.quit()

email = config.get('DEFAULT', 'email')
X = config.getint('DEFAULT', 'X')

"""verificam care preturi sunt < X"""
for title, price in info_titlu_pret:
    if int(price) < X:
        send_email(email, title, price)