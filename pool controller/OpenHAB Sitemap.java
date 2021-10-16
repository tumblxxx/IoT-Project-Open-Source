sitemap default label="Windows 10 IoT"
{
    Frame label="" 
    {
      Text label="Pool" icon="swimmingpool" {
			Switch item=PoolPump mappings=[ON="ON", OFF="OFF"]
			Switch item=WaterFall mappings=[ON="ON", OFF="OFF"]
			Switch item=PoolLights mappings=[ON="ON", OFF="OFF"]
			Text item=pooltemp 
			Text item=solartemp 
			Text item=outsidetemp 
		}
    }       
}
