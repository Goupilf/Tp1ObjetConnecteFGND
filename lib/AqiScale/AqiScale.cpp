#include "AqiScale.h"

String AqiScale::getAqi(int pmsVal){
 if (this.responseText <= 11) {
    return "La qualité de l’air est considérée comme satisfaisante et la pollution de l’air pose peu ou pas de risque.";
 }
 else if (this.responseText > 11 && this.responseText <= 34) {
    return "La qualité de l’air est acceptable; cependant, pour certains polluants, il peut y avoir un problème de santé modéré pour un très petit nombre de personnes qui sont inhabituellement sensibles à la pollution atmosphérique.";
 }
 else if (this.responseText > 34 && this.responseText <= 54) {
    return "Les membres de groupes sensibles peuvent subir des effets sur la santé. Le grand public ne sera probablement pas affecté.";
 }
 else if (this.responseText > 54) {
    return "Tout le monde peut commencer à ressentir des effets sur la santé; les membres de groupes sensibles peuvent subir des effets plus graves sur la santé.";
 }
}