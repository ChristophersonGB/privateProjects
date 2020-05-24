#selenium tutorial 
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

PATH = "C:\Program Files (x86)\chromedriver.exe" # path set to wherever our chromedriver.exe is
driver = webdriver.Chrome(PATH) #can be any (mostly) browser

driver.get("https://techwithtim.net")
print(driver.title)

search = driver.find_element_by_name("s")
search.send_keys("test")
search.send_keys(Keys.RETURN)

try: 
    main = WebDriverWait(driver, 10).until(EC.presence_of_all_elements_located((By.ID, "main")))
    print(main.text)
except:
    driver.quit()


driver.quit()