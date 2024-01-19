#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <chrono>
#include <systemd/sd-bus.h>
#include <systemd/sd-event.h>
//#include <mapper.h>

//static void quit(int r, void *loop)
//{
//	sd_event_exit((sd_event *)loop, r);
//}

std::chrono::_V2::system_clock::time_point start, finish;
int cnt = 0;

static int callback(sd_bus_message *m, void *user, sd_bus_error *error)
{
	char *name = (char *)user;
	int r;
	char *property = NULL;
  char *property2 = NULL;
  const char *contents;
  double dvalue;

    printf("%s ", name);


  /*r = sd_bus_message_peek_type(m, NULL, &contents); if (r < 0) return r;
  printf("initial contents=%s\n", contents);*/


	//r = sd_bus_message_skip(m, "s");
  r = sd_bus_message_read(m, "s", &property);
      printf(" %s ", property);
	if (r < 0) {
		fprintf(stderr, "Error skipping message fields: %s\n",
				strerror(-r));
		//quit(r, loop);
		return r;
	}

/*r = sd_bus_message_peek_type(m, NULL, &contents); if (r < 0) return r;
  printf("initial2 contents=%s\n", contents);*/

    //printf("entering container ARRAY {sv}\n");

	r = sd_bus_message_enter_container(m, SD_BUS_TYPE_ARRAY, "{sv}");
	if (r < 0) {
		fprintf(stderr, "Error entering container: %s\n",
				strerror(-r));
		//quit(r, loop);
		return r;
	}

/*r = sd_bus_message_peek_type(m, NULL, &contents); if (r < 0) return r;
  printf("initial3 contents=%s\n", contents);*/


    //printf("loop entering container DICT {sv}\n");

	while((r = sd_bus_message_enter_container(
					m,
					SD_BUS_TYPE_DICT_ENTRY,
					"sv")) > 0) {

    //printf("reading s property\n");

		r = sd_bus_message_read(m, "s", &property);
    printf("%s ", property);
		if (r < 0) {
			fprintf(stderr, "Error reading message: %s\n",
					strerror(-r));
			//quit(r, loop);
			return r;
		}

		if(!strcmp(property, "Value")) 
    {
      //printf("finished strcmp\n"); 
          r = sd_bus_message_peek_type(m, NULL, &contents); if (r < 0) return r;
          //printf("contents=%s ", contents);
          r = sd_bus_message_enter_container(m, SD_BUS_TYPE_VARIANT, NULL); if (r < 0) return r;
          if ((contents[0] == 'd')&&(contents[1] == 0)) {
            r = sd_bus_message_read_basic(m, SD_BUS_TYPE_DOUBLE, &dvalue); if (r < 0) return r;
            printf("value=%f ", dvalue);
            finish = std::chrono::high_resolution_clock::now();
            int64_t msec = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
            printf("msec=%ld ", msec);
            cnt++;
            printf("cnt=%d callspersec=%f\n", cnt, (double)cnt/msec);
          }
			continue;
    }

    printf(" another property=%s\n", property);


		//quit(0, loop);
		//break;
	}
  /*printf("leaving container DICT {sv}\n");
  printf("entering container ARRAY s}\n");
  r = sd_bus_message_peek_type(m, NULL, &contents); if (r < 0) return r;
  printf("leaved contents=%s ", contents);*/





	return 0;
}

int main(int argc, char *argv[])
{
	static const char *matchfmt =
		"type='signal',"
		"interface='org.freedesktop.DBus.Properties',"
		"member='PropertiesChanged',"
		"path='%s',"
		"sender='%s'";
	static const char *usage =
		"Usage: %s\n";
	static const size_t LEN = 256;

	sd_bus *conn = NULL;
	sd_event *loop = NULL;
	sd_bus_slot *slot = NULL;
	sd_bus_error error = SD_BUS_ERROR_NULL;
  char *path[] = {


        (char *)("/xyz/openbmc_project/sensors/voltage/P12V"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P1V05_PCH"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P3V3"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P5V"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P5V_STBY"),
        (char *)("/xyz/openbmc_project/sensors/voltage/PVCCIN_CPU0"),
        (char *)("/xyz/openbmc_project/sensors/voltage/PVCCIN_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_1V0_AUX_LAN"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_1V8_PCH_AUX"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_VDDQ_CPU0_ABCD"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_VDDQ_CPU0_EFGH"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_VDDQ_CPU1_IJKL"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_VDDQ_CPU1_MNOP"),
        (char *)("/xyz/openbmc_project/sensors/voltage/P_VNN_PCH_AUX"),
/* 14 */  
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_11_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_12_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_13_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_15_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_16_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_17_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_18_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_19_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_1_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_20_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_21_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_22_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_25_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_26_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_27_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_28_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_2_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_30_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_31_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_7_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_8_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Core_9_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/DIMM_G1_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/DTS_CPU1"),
        (char *)("/xyz/openbmc_project/sensors/temperature/Die_CPU1"),
/* 25 */        
  };
  int pathlen[] = { 14, 25 }; 
	char *service[] = { (char *)("xyz.openbmc_project.ADCSensor"),
                      (char *)("xyz.openbmc_project.IntelCPUSensor"),
  /* 2 */
  };
  int servlen = 2;
	int r; /*dest = -1, */
  double state;
	char match[LEN];

	if(argc > 1) {
		fprintf(stderr, usage, argv[0]);
		exit(EXIT_FAILURE);
	}

	//if(!strcmp(argv[2], "on"))
	//	dest = 1;
	//if(!strcmp(argv[2], "off"))
	//	dest = 0;

	//if(dest != 0 && dest != 1) {
	//	fprintf(stderr, usage, argv[0]);
	//	exit(EXIT_FAILURE);
	//}

	r = sd_bus_default_system(&conn);
	if(r < 0) {
		fprintf(stderr, "Error connecting to system bus: %s\n",
				strerror(-r));
		goto finish;
	}

	//r = mapper_get_service(conn, argv[1], &service);
	//if (r < 0) {
	//	fprintf(stderr, "Error obtaining host service: %s\n",
	//			strerror(-r));
	//	goto finish;
	//}

	r = sd_event_default(&loop);
	if (r < 0) {
		fprintf(stderr, "Error obtaining event loop: %s\n",
				strerror(-r));
		goto finish;
	}

	r = sd_bus_attach_event(conn, loop, SD_EVENT_PRIORITY_NORMAL);
	if (r < 0) {
		fprintf(stderr, "Failed to attach system "
				"bus to event loop: %s\n",
				strerror(-r));
		goto finish;
	}
  for (int j = 0, i = 0; j < servlen; j++) {
   for (; i < pathlen[j]; i++) {

	  if(strlen(matchfmt) + strnlen(path[i], LEN) > LEN) {
		  r = -E2BIG;
		  fprintf(stderr, "Error adding match rule: %s\n",
			  	strerror(-r));
		  goto finish;
	  }

	  sprintf(match, matchfmt, path[i], service[j]);

	  r = sd_bus_add_match(conn,
		  	&slot,
			  match,
			  callback,
			  path[i]);
	  if(r < 0) {
		  fprintf(stderr, "Error adding match rule: %s\n",
			  	strerror(-r));
		  goto finish;
	  }
    printf("getting property %s %s\n", service[j], path[i]);
	  r = sd_bus_get_property_trivial(conn,
			service[j],
			path[i],
			"xyz.openbmc_project.Sensor.Value",
			"Value",
			&error,
			'd',
			&state);
	  if(r < 0) {
		  fprintf(stderr, "Error getting property: %s\n",
			  	strerror(-r));
		  goto finish;
	  }
    printf("property state =%f\n", state);
   }
  }
    //printf("entering sd_event_loop\n");
  start = std::chrono::high_resolution_clock::now();
	r = sd_event_loop(loop);
	if(r < 0) {
		fprintf(stderr, "Error starting event loop: %s\n",
				strerror(-r));
		goto finish;
	}
    //printf("leaving sd_event_loop\n");

finish:
	exit(r < 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
