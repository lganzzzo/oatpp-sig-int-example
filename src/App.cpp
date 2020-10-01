#include "./controller/MyController.hpp"
#include "./AppComponent.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>
#include <signal.h>

void intHandler(int s){
  OATPP_LOGI("MyApp", "Sig received!");
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::Server>, server);
  server->stop();
}

void run() {

  /* Register Components in scope of run() method */
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Create MyController and add all of its endpoints to router */
  auto myController = std::make_shared<MyController>();
  myController->addEndpointsToRouter(router);

  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::Server>, server);

  /* Priny info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

  /* Run server */
  server->run();

  connectionHandler->stop();
  connectionProvider->stop();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {

  //////////////////////////////////////////////////////////////////////////////
  // Handle interrupt signal (CMD + C)

  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = intHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  //////////////////////////////////////////////////////////////////////////////

  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();
  
  return 0;

}
