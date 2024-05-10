import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
file ="Humidity.csv"   
df=pd.read_csv(file)
sns.lineplot(x=df.index, y='Humidity', data=df)
plt.title('Humidity over Time')
plt.xlabel('Time(in hour)')
plt.ylabel('Humidity(in %)')
plt.show()

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
file ="Temperature.csv"   
df=pd.read_csv(file)
sns.lineplot(x=df.index, y='Temperature, data=df)
plt.title('Temperature over Time')
plt.xlabel('Time(in hour)')
plt.ylabel('Temperature(in °C)')
plt.show()

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
file ="AQI.csv"   
df=pd.read_csv(file)
sns.lineplot(x=df.index, y='AQI', data=df)
plt.title('Air Quality Index over Time')
plt.xlabel('Time(in hour)')
plt.ylabel('Air Quality Index(in ppm)')
plt.show()

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
file ="UV Index.csv"   
df=pd.read_csv(file)
sns.lineplot(x=df.index, y='UV Index', data=df)
plt.title('UV Index over Time')
plt.xlabel('Time(in hour)')
plt.ylabel('UV Index')
plt.show()
