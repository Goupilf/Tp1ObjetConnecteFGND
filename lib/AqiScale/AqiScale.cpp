#include "AqiScale.h"

AqiScale::AqiScale(){}

String AqiScale::getAqi(int pmsVal){
 if (pmsVal <= 11) {
    return "La qualité de l’air est considérée comme satisfaisante et la pollution de l’air pose peu ou pas de risque.";
 }
 else if (pmsVal <= 34) {
    return "La qualité de l’air est acceptable; cependant, pour certains polluants, il peut y avoir un problème de santé modéré pour un très petit nombre de personnes qui sont inhabituellement sensibles à la pollution atmosphérique.";
 }
 else if (pmsVal <= 54) {
    return "Les membres de groupes sensibles peuvent subir des effets sur la santé. Le grand public ne sera probablement pas affecté.";
 }
 else {
    return "Tout le monde peut commencer à ressentir des effets sur la santé; les membres de groupes sensibles peuvent subir des effets plus graves sur la santé.";
 }
}
String AqiScale::getAqiLabel(int pmsVal){
 if (pmsVal <= 11) {
    return "Bon";
 }
 else if (pmsVal <= 34) {
    return "Acceptable";
 }
 else if (pmsVal <= 54) {
    return "Mauvais pour les groupes sensibles";
 }
 else {
    return "Mauvais pour la santé";
 }
}