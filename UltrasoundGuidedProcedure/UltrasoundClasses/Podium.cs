using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Podium : MonoBehaviour
{
    [SerializeField] private Vector3 GameLocation;
    [SerializeField] private GameObject WallOne;
    //[SerializeField] private GameObject WallTwo;
    //[SerializeField] private GameObject WallThree;

    [SerializeField] private GameObject[] OrbitWalls;

    [SerializeField] private FloatingGameManger CurrentManager;

    [SerializeField] private AudioSource PodiumAudioSource;
    [SerializeField] private AudioClip GearTurningSound;
    private bool bIsSpawned;

    [SerializeField] private float WallOneTimer;
    [SerializeField] private float WallTwoTimer;
    [SerializeField] private float WallThreeTimer;
    public float podiumLocationMErr;

    private bool bSpawnedTestGems = false;

    private bool bWallsSpawned;

    private bool bInitialMovement;

    [SerializeField] private bool bPodiumSet;

    [SerializeField] private bool bMoveDown;
    // Start is called before the first frame update
    void Start()
    {
        WallOne.SetActive(false);
        bPodiumSet = true;
    }

    // Update is called once per frame
    void Update()
    {
        
        // // debugging the location
        // GameLocation.y = LineIntersection.podiumLocation.y+podiumLocationMErr;
        // transform.position = Vector3.MoveTowards(transform.position, GameLocation, 1.0f * Time.deltaTime);
        
        // Handle setting and adjusting podium location
        if (bIsSpawned && transform.position != GameLocation)
        {
            GameLocation.y = LineIntersection.podiumLocation.y+podiumLocationMErr;
            transform.position = Vector3.MoveTowards(transform.position, GameLocation, 1.0f * Time.deltaTime);
        }
        if (transform.position == GameLocation && !bSpawnedTestGems)
        {
            PodiumAudioSource.Stop();
            bSpawnedTestGems = true;
            CurrentManager.SetPodiumInPosition(true);
        }
        if (!bPodiumSet)
        {
            GameLocation.y = LineIntersection.podiumLocation.y+podiumLocationMErr;
            transform.position = Vector3.MoveTowards(transform.position, GameLocation, 1.0f * Time.deltaTime);
        }
    }

    public void SetIsSpawned(bool NewBool)
    {
        bIsSpawned = NewBool;

        if (bIsSpawned)
        {
            PodiumAudioSource.clip = GearTurningSound;
            PodiumAudioSource.Play();
        }
    }

    public void ActivateWalls()
    {
        bWallsSpawned = true;
        
        WallOne.SetActive(true);

        WallOneTimer = 2.0f;
        WallTwoTimer = 4.0f;
        WallThreeTimer = 6.0f;
    }

    public void ActivateOrbitWalls()
    {
        foreach (GameObject OrbitWall in OrbitWalls)
        {
            OrbitWall.SetActive(true);
        }
    }

    public void SetPodium(bool NewBool)
    {
        bPodiumSet = NewBool;
    }
}
