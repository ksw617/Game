using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
public class Character : MonoBehaviour
{
    [SerializeField] private NavMeshAgent meshAgent; 
    [SerializeField] private CharacterInfo characterInfo;
    int childIndex;

    private void Awake()
    {
        meshAgent = GetComponent<NavMeshAgent>();
    }
    public void SetCharacterInfo(CharacterInfo info)
    {
        characterInfo = info;
        childIndex = GameInfo.Instance.characterNames[characterInfo.characterShape];
        transform.GetChild(childIndex).gameObject.SetActive(true);
        gameObject.name = characterInfo.name;
        meshAgent.speed = characterInfo.moveSpeed;
        gameObject.SetActive(true);
    }

    private void OnDisable()
    {
        transform.GetChild(childIndex).gameObject.SetActive(false);
    }


}
